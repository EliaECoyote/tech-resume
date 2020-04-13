open Lwt.Infix;
open Graphql_lwt;

type role =
  | User
  | Admin;

type user = {
  id: int,
  name: string,
  role,
  friends: list(user),
};

let rec alice = {id: 1, name: "Alice", role: Admin, friends: [bob]}
and bob = {id: 2, name: "Bob", role: User, friends: [alice]};

let users = [alice, bob];

let role =
  Schema.(
    enum(
      "role",
      ~values=[
        enum_value("USER", ~value=User, ~doc="A regular user"),
        enum_value("ADMIN", ~value=Admin, ~doc="An admin user"),
      ],
    )
  );

let user =
  Schema.(
    obj("user", ~fields=user =>
      [
        field("id", ~args=Arg.[], ~typ=non_null(int), ~resolve=(_, p) =>
          p.id
        ),
        field("name", ~args=Arg.[], ~typ=non_null(string), ~resolve=(_, p) =>
          p.name
        ),
        field("role", ~args=Arg.[], ~typ=non_null(role), ~resolve=(_, p) =>
          p.role
        ),
        field(
          "friends",
          ~args=Arg.[],
          ~typ=list(non_null(user)),
          ~resolve=(_, p) =>
          Some(p.friends)
        ),
      ]
    )
  );

let rec consume_stream = stream =>
  Lwt.catch(
    () =>
      Lwt_stream.next(stream)
      >>= (
        x => {
          let Ok(x) | Error(x) = x;
          Printf.eprintf(
            "stream response: '%s'\n%!",
            Yojson.Basic.to_string(x),
          );
          if (Lwt_stream.is_closed(stream)) {
            Lwt.return_unit;
          } else {
            consume_stream(stream);
          };
        }
      ),
    fun
    | Lwt_stream.Closed
    | Lwt_stream.Empty => Lwt.return_unit
    | _ => Lwt.return_unit,
  );

let set_interval = (s, f, destroy) => {
  let rec set_interval_loop = (s, f, n) => {
    let timeout =
      Lwt_timeout.create(s, () =>
        if (n > 0) {
          f();
          set_interval_loop(s, f, n - 1);
        } else {
          destroy();
        }
      );

    Lwt_timeout.start(timeout);
  };

  set_interval_loop(s, f, 5);
};

let schema =
  Schema.(
    schema(
      [
        io_field(
          "users",
          ~args=Arg.[],
          ~typ=non_null(list(non_null(user))),
          ~resolve=(_, ()) =>
          Lwt_result.return(users)
        ),
        field(
          "greeter",
          ~typ=string,
          ~args=
            Arg.[
              arg(
                "config",
                ~typ=
                  non_null(
                    obj(
                      "greeter_config",
                      ~coerce=(greeting, name) => (greeting, name),
                      ~fields=[
                        arg'("greeting", ~typ=string, ~default="hello"),
                        arg("name", ~typ=non_null(string)),
                      ],
                    ),
                  ),
              ),
            ],
          ~resolve=(_, (), (greeting, name)) =>
          Some(Format.sprintf("%s, %s", greeting, name))
        ),
      ],
      ~subscriptions=[
        subscription_field(
          "subscribe_to_user",
          ~typ=non_null(user),
          ~args=Arg.[arg'("intarg", ~typ=int, ~default=42)],
          ~resolve=(_info, _intarg) => {
            let (user_stream, push_to_user_stream) = Lwt_stream.create();
            let destroy_stream = () => push_to_user_stream(None);
            set_interval(
              2,
              () => {
                let idx = Random.int(List.length(users));
                push_to_user_stream(Some(List.nth(users, idx)));
              },
              destroy_stream,
            );
            Lwt_result.return((user_stream, destroy_stream));
          },
        ),
      ],
    )
  );

module Graphql_cohttp_lwt =
  Graphql_cohttp.Make(
    Graphql_lwt.Schema,
    Cohttp_lwt_unix.IO,
    Cohttp_lwt.Body,
  );

let start = () => {
  Console.log("start");
  open Printf;
  let on_exn =
    fun
    | [@implicit_arity] Unix.Unix_error(error, func, arg) =>
      printf(
        "Client connection error %s: %s(%S)",
        Unix.error_message(error),
        func,
        arg,
      )
    | exn => printf("Unhandled exception: %s\n%!", Printexc.to_string(exn));

  let callback = Graphql_cohttp_lwt.make_callback(_req => (), schema);
  let server = Cohttp_lwt_unix.Server.make_response_action(~callback, ());
  let port = 8080;
  let mode = `TCP(`Port(port));
  printf("listening on http://localhost:%d/graphql\n%!", port);
  Cohttp_lwt_unix.Server.create(~on_exn, ~mode, server) |> Lwt_main.run;
};
Console.log("test");
