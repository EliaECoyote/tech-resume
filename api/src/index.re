// TODO: use ENV vars for this
let protocol = "http";
let host = "127.0.0.1";
let port = 3000;

let app = Express.express();

let mainMiddleware =
  Express.Middleware.from((next, req) =>
    Express.Response.sendString("hello world")
  );

let onListen = e =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => Js.log @@ {j|Listening at $protocol://$host:$port|j}
  };

Express.App.get(app, ~path="/", mainMiddleware);

Express.App.listen(app, ~port, ~onListen, ());