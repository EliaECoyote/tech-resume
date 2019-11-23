let buildExpressResponse =
    (res: Express.Response.t, result: MarkdownToHtmlConverter.result) => {
  switch (result) {
  | Ok(data) => Express.Response.sendString(data, res)
  | Error(_) =>
    Express.Response.status(Express.Response.StatusCode.BadRequest, res)
    |> Express.Response.sendString("Incorrect / malformed request")
  };
};

let middleware =
  Express.PromiseMiddleware.from((_, req, res) => {
    let query = Express.Request.query(req);
    let src =
      Js.Dict.get(query, "md")
      ->Belt.Option.flatMap(Js.Json.decodeString)
      ->Belt.Option.getWithDefault("");
    let template = MarkdownToHtmlConverter.Standard;
    let toResponse = buildExpressResponse(res);
    MarkdownToHtmlConverter.run(~src, ~template)
    |> Js.Promise.then_(result => result->toResponse->Js.Promise.resolve);
  });