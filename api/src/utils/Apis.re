let fetchPdfConversion = (~html) =>
  Url.make(~endpoint=Config.pdfgenEndpoint, ())
  |> HttpClient.post(
       ~resource=_,
       ~body=Fetch.BodyInit.make(html),
       ~headers=
         Fetch.HeadersInit.makeWithArray([|("Content-Type", "text/html")|]),
     )
  |> Wonka.mergeMap((. value) => HttpClient.toBuffer(value));