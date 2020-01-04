let port = Config.port;

let onListen = e =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => Js.log @@ {j|server started at port $port|j}
  };

let startupBackend = () => {
  let app = Express.express();
  Express.App.use(app, CorsMiddleware.middlewareFactory());
  Express.App.use(app, PdfGeneratorMiddleware.middleware);
  Express.App.listen(app, ~port, ~onListen, ());
};

startupBackend();