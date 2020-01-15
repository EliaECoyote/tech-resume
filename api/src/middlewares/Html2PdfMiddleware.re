type html2pdfResult('a) =
  | HtmlNotFound
  | PdfConversionFailure
  | PdfConversionSuccess('a);

let html2pdf = (~html=?, ()) =>
  switch (html) {
  | Some(html) =>
    Apis.fetchPdfConversion(~html)
    |> Wonka.map((. result) =>
         switch (result) {
         | HttpClient.Ok(value) => PdfConversionSuccess(value)
         // TODO: failure should forward the status code received in
         // the pdf service response
         | HttpClient.FailureCode(code) =>
           Js.Console.error @@ {j| pdf conversion failed with status $code |j};
           PdfConversionFailure;
         | HttpClient.Failure =>
           Js.Console.error @@ {j| pdf conversion failed with generic error |j};
           PdfConversionFailure;
         }
       )
  | None => Wonka.fromValue(HtmlNotFound)
  };

let middleware =
  Express.PromiseMiddleware.from((_, req, res) => {
    Wonka.fromValue(req)
    |> Wonka.map((. req) =>
         Express.Request.asJsonObject(req)
         // grabs html template from the request object
         |> Js.Dict.get(_, "html")
         |> Belt.Option.flatMap(_, Js.Json.decodeString)
       )
    |> Wonka.mergeMap((. html) => html2pdf(~html?, ()))
    |> Wonka.map((. result) =>
         switch (result) {
         | HtmlNotFound =>
           res
           |> Express.Response.status(
                Express.Response.StatusCode.InternalServerError,
              )
           |> Express.Response.sendString(
                "pdf generation service was not able to access html",
              )
         // TODO: failure should forward the status code received in
         // the pdf service response
         | PdfConversionFailure =>
           res
           |> Express.Response.status(Express.Response.StatusCode.BadRequest)
           |> Express.Response.sendString("[api] pdf generation failed")
         | PdfConversionSuccess(value) =>
           res
           |> Express.Response.status(Express.Response.StatusCode.Ok)
           |> Express.Response.setHeader(
                "Content-Type",
                "application/octet-stream",
              )
           |> Express.Response.sendBuffer(value)
         }
       )
    |> Wonka.toPromise
  });