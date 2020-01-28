let middleware =
  Express.Middleware.fromError((_, err, _, res) => {
    Js.Console.error(err);
    switch (err) {
    | Failure(f) =>
      res
      |> Express.Response.status(Express.Response.StatusCode.BadRequest)
      |> Express.Response.sendString(f)
    | _ =>
      res
      |> Express.Response.sendStatus(
           Express.Response.StatusCode.InternalServerError,
         )
    };
  });