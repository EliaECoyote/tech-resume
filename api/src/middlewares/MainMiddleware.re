let buildExpressResponse =
    (res: Express.Response.t, result: ChildProcess.spawnResult) => {
  switch (result) {
  | Belt.Result.Ok(data) => Express.Response.sendString(data, res)
  | Belt.Result.Error(data) =>
    Express.Response.status(Express.Response.StatusCode.BadRequest, res)
    |> Express.Response.sendString("Incorrect request data. Error: " ++ data)
  };
};

let mainMiddleware =
  Express.PromiseMiddleware.from((_, _, res) =>
    Pandoc.run(~src="# Test", ~fromKind=Pandoc.Markdown, ~toKind=Pandoc.Html)
    |> Js.Promise.then_(result =>
         buildExpressResponse(res, result)->Js.Promise.resolve
       )
  );