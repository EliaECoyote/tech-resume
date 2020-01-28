let port = Config.port;

let startupBackend = () => {
  let app = Express.express();
  let bodyParserOptions: BodyParser.textOptions = {
    defaultCharset: None,
    inflate: None,
    limit: None,
    type_: Some("text/plain"),
  };

  let onListen = e =>
    switch (e) {
    | exception (Js.Exn.Error(e)) =>
      Js.Console.error(e);
      Node.Process.exit(1);
    | _ => Js.log @@ {j|Listening at port $port|j}
    };

  Express.App.postWithMany(
    app,
    ~path="/convert",
    [|
      CorsMiddleware.middlewareFactory(),
      BodyParser.textWithOptions(bodyParserOptions),
      Md2HtmlConverterMiddleware.middleware,
      Html2PdfMiddleware.middleware,
      ErrorMiddleware.middleware,
    |],
  );

  Express.App.listen(app, ~port, ~onListen, ());
};

startupBackend();