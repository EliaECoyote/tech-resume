let fetchHtmlConversion = (~md) => {
  let url =
    Url.make(
      ~scheme="http",
      ~host="127.0.0.1:3000",
      ~path="convert",
      ~qsComponents=[|("md", md)|],
      (),
    );
  HttpClient.get(~resource=url)
  |> Wonka.mergeMap((. value) => HttpClient.toText(value));
};