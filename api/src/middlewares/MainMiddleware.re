let buildExpressResponse =
    (res: Express.Response.t, result: ChildProcess.result) => {
  switch (result) {
  | Belt.Result.Ok(data) => Express.Response.sendString(data, res)
  | Belt.Result.Error(data) => Express.Response.sendString(data, res)
  };
};

let mainMiddleware =
  Express.PromiseMiddleware.from((next, req, res) =>
    Pandoc.run(~src="# Test", ~fromKind=Pandoc.Markdown, ~toKind=Pandoc.Html)
    |> Js.Promise.then_(result =>
         buildExpressResponse(res, result)->Js.Promise.resolve
       )
  );