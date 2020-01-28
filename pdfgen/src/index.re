let port = Config.port;

let onListen = e =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => Js.log @@ {j|Listening at port $port|j}
  };

let startupBackend = () => {
  let app = Express.express();
  let bodyParserOptions: BodyParser.textOptions = {
    defaultCharset: None,
    inflate: None,
    limit: None,
    type_: Some("text/html"),
  };
  Express.App.postWithMany(
    app,
    ~path="/",
    [|
      CorsMiddleware.middlewareFactory(),
      BodyParser.textWithOptions(bodyParserOptions),
      PdfGeneratorMiddleware.middleware,
    |],
  );
  Express.App.listen(app, ~port, ~onListen, ());
};

startupBackend();