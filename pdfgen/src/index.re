// TODO: use ENV vars for this
let protocol = "http";
let host = "127.0.0.1";
let port = 4000;

let startupBackend = () => {
  let app = Express.express();

  let onListen = e =>
    switch (e) {
    | exception (Js.Exn.Error(e)) =>
      Js.log(e);
      Node.Process.exit(1);
    | _ => Js.log @@ {j|Listening at $protocol://$host:$port|j}
    };

  Express.App.use(app, CorsMiddleware.middlewareFactory());
  Express.App.use(app, PdfGeneratorMiddleware.middleware);
  Express.App.listen(app, ~port, ~onListen, ());
};

startupBackend();