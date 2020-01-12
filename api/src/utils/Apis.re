let fetchPdfConversion = (~html) =>
  Url.make(
    ~endpoint=Config.pdfgenEndpoint,
    ~qsComponents=[|("html", html)|],
    (),
  )
  |> HttpClient.get(~resource=_)
  |> Wonka.mergeMap((. value) => HttpClient.toText(value));