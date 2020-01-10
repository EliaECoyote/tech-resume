let buildExpressResponse =
    (res: Express.Response.t, result: MarkdownToHtmlConverter.result) => {
  switch (result) {
  | Ok(data) => Express.Response.sendString(data, res)
  | Error(_) =>
    Express.Response.status(Express.Response.StatusCode.BadRequest, res)
    |> Express.Response.sendString("Incorrect / malformed request")
  };
};

type html2pdfResult('a) =
  | HtmlNotFound
  | PdfConversionFailure
  | PdfConversionSuccess('a);

let middleware =
  Express.PromiseMiddleware.from((_, req, res) => {
    Wonka.fromValue(req)
    |> Wonka.map((. req) =>
         Express.Request.asJsonObject(req)
         |> Js.Dict.get(_, "html")
         |> Belt.Option.flatMap(_, Js.Json.decodeString)
       )
    |> Wonka.mergeMap((. html) =>
         switch (html) {
         | Some(html) =>
           Apis.fetchPdfConversion(~html)
           |> Wonka.map((. result) =>
                switch (result) {
                | HttpClient.Ok(value) => PdfConversionSuccess(value)
                // TODO: failure should forward the status code received in
                // the pdf service response
                | _ => PdfConversionFailure
                }
              )
         | None => Wonka.fromValue(HtmlNotFound)
         }
       )
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
         | PdfConversionFailure =>
           res
           |> Express.Response.status(Express.Response.StatusCode.BadRequest)
           |> Express.Response.sendString(
                "pdf generation failed - check again your input",
              )
         | PdfConversionSuccess(value) =>
           res
           |> Express.Response.status(Express.Response.StatusCode.Ok)
           |> Express.Response.sendBuffer(value |> Node.Buffer.fromString)
         }
       )
    |> Wonka.toPromise
  });