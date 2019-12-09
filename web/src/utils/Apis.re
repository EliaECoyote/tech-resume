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

let fetchPdfConversion = (~html) => {
  let url =
    Url.make(
      ~scheme="http",
      ~host="127.0.0.1:4000",
      ~path="",
      ~qsComponents=[|("html", html)|],
      (),
    );
  // let data = encodePdfConversionData(~contents_=html);
  // switch (data) {
  // | Belt.Result.Ok(value) =>
  // let body = Fetch.BodyInit.make(value);
  HttpClient.get(~resource=url);
  // | Belt.Result.Error(value) =>
  // Js.log((value, html));
  // Wonka.fromValue(HttpClient.Failure);
  // };
};