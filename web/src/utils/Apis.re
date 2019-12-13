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
  |> Wonka.concatMap((. value) => HttpClient.toText(value));
};

// let fetchPdf = (~html) => {
//   let url = Url.make(~scheme="http", ~host="127.0.0.1:5001", ~path="", ());
//   // let body = Fetch.
//   HttpClient.post((), ());
// };

let encodePdfConversionData = (~contents_: string) => {
  Global.stringToBase64(contents_)
  ->Belt.Result.flatMap(encodedContents => {
      let encodedData =
        Js.Json.stringifyAny({
          "contents": encodedContents,
          "options": Js.Obj.empty(),
        });
      switch (encodedData) {
      | Some(value) => Belt.Result.Ok(value)
      | None => Belt.Result.Error("cannot encode pdf conversion request body")
      };
    });
};