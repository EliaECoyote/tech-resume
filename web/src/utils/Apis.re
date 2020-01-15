let fetchPdfData = (~md) =>
  Url.make(
    ~scheme=Config.apiScheme,
    ~host=Config.apiHost,
    ~path="convert",
    ~qsComponents=[|("md", md)|],
    (),
  )
  |> HttpClient.get(~resource=_)
  |> Wonka.mergeMap((. value) => HttpClient.toArrayBuffer(value));