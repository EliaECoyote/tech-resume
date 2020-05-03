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

let schema =
  Schema.(
    schema([
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
    ])
  );

module Graphql_cohttp_lwt =
  Graphql_cohttp.Make(
    Graphql_lwt.Schema,
    Cohttp_lwt_unix.IO,
    Cohttp_lwt.Body,
  );

let start = () => {
  let on_exn =
    fun
    | [@implicit_arity] Unix.Unix_error(error, func, arg) => {
        let message = Unix.error_message(error);
        Console.error(
          "Client connection error "
          ++ message
          ++ ": "
          ++ func
          ++ "("
          ++ arg
          ++ ")",
        );
      }
    | exn => Console.error(("Unhandled exception", exn));

  let callback = Graphql_cohttp_lwt.make_callback(_req => (), schema);
  let server = Cohttp_lwt_unix.Server.make_response_action(~callback, ());
  let port = int_of_string(Sys.getenv("PORT"));
  let mode = `TCP(`Port(port));
  Console.log(
    "listening on http://localhost:" ++ string_of_int(port) ++ "/graphql!",
  );
  Cohttp_lwt_unix.Server.create(~on_exn, ~mode, server) |> Lwt_main.run;
};