let buildExpressResponse =
    (res: Express.Response.t, result: ChildProcess.spawnResult) => {
  switch (result) {
  | Ok(data) => Express.Response.sendString(data, res)
  | Error(data) =>
    Express.Response.status(Express.Response.StatusCode.BadRequest, res)
    |> Express.Response.sendString("Incorrect request data. Error: " ++ data)
  };
};

let middleware =
  Express.PromiseMiddleware.from((_, _, res) =>
    MarkdownToHtmlConverter.run(
      ~src="# Test \n ## heyy",
      ~template=MarkdownToHtmlConverter.Standard,
    )
    |> Js.Promise.then_((result: ChildProcess.spawnResult) =>
         buildExpressResponse(res, result)->Js.Promise.resolve
       )
  );