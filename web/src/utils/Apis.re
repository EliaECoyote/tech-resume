let fetchPdfData = (~md) =>
  Url.make(
    ~scheme=Config.apiScheme,
    ~host=Config.apiHost,
    ~path="convert",
    (),
  )
  |> HttpClient.post(
       ~resource=_,
       ~body=Fetch.BodyInit.make(md),
       ~headers=
         Fetch.HeadersInit.makeWithArray([|("Content-Type", "text/plain")|]),
     )
  |> Wonka.mergeMap((. value) => HttpClient.toArrayBuffer(value));