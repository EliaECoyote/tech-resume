let port = Config.port;

let startupBackend = () => {
  let app = Express.express();

  let onListen = e =>
    switch (e) {
    | exception (Js.Exn.Error(e)) =>
      Js.log(e);
      Node.Process.exit(1);
    | _ => Js.log @@ {j|Listening at port $port|j}
    };

  Express.App.getWithMany(
    app,
    ~path="/convert",
    [|
      CorsMiddleware.middlewareFactory(),
      Md2HtmlConverterMiddleware.middleware,
      Html2PdfMiddleware.middleware,
      ErrorMiddleware.middleware,
    |],
  );

  Express.App.listen(app, ~port, ~onListen, ());
};

startupBackend();