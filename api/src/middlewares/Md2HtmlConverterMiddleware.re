exception ConversionFailed(string);

let middleware =
  Express.PromiseMiddleware.from((next, req, res) => {
    let template = MarkdownToHtmlConverter.Standard;
    Express.Request.bodyText(req)
    |> Belt.Option.getWithDefault(_, "")
    |> MarkdownToHtmlConverter.run(~src=_, ~template)
    |> Wonka.map((. result) =>
         switch (result) {
         | MarkdownToHtmlConverter.Ok(value) =>
           // ⚠️ sets conversion result in the request *html* field
           Express.Request.asJsonObject(req)
           |> Js.Dict.set(_, "html", Js.Json.string(value));
           // invoke next middleware
           next(Express.Next.middleware, res);
         | MarkdownToHtmlConverter.Error(value) =>
           // invoke next error-handler middleware
           ConversionFailed(value) |> Express.Next.error |> next(_, res)
         }
       )
    |> Wonka.toPromise;
  });