type fileNameKind =
  | Html
  | Pdf;

let generateFileName = kind => {
  NodeCrypto.randomBytes(16)
  |> Node.Buffer.toString
  |> (
    filename =>
      switch (kind) {
      | Html => filename ++ ".html"
      | Pdf => filename ++ ".pdf"
      }
  );
};

// let createTmpHtmlFile = html => {
//   let fileName =
//     NodeCrypto.randomBytes(16)
//     |> Node.Buffer.toString
//     |> (value => value ++ ".html");
//   Node.Fs.writeFileSync(fileName, html, `utf8);
//   fileName;
// };

let deleteFile = Node.Fs.unlinkSync;

[@bs.send]
external bufferToString: (Node.Buffer.t, string) => string = "toString";

let middleware =
  Express.PromiseMiddleware.from((_, req, res) =>
    Express.Request.query(req)
    |> Wonka.fromValue
    |> Wonka.map((. query) =>
         Js.Dict.get(query, "html")
         ->Belt.Option.flatMap(Js.Json.decodeString)
         ->Belt.Option.getWithDefault("")
       )
    |> Wonka.mergeMap((. html) => {
         let pdfFileName = generateFileName(Pdf);
         ChildProcess.spawn(
           ~path="wkhtmltopdf",
           ~args=[|pdfFileName|],
           ~src=html,
         )
         |> Wonka.fromPromise
         |> Wonka.map((. result) =>
              switch (result) {
              | ChildProcess.Ok(value) =>
                Node.Fs.readFileSync(pdfFileName, `utf8)
                ->Node.Buffer.fromString
                ->bufferToString("base64")
                ->(base64 => Ok(base64))
              | ChildProcess.Error(value) => Error(value)
              }
            );
       })
    |> Wonka.toPromise
    |> Js.Promise.then_(result => {
         let response =
           switch (result) {
           | Ok(value) => Express.Response.sendString(value, res)
           | Error(value) =>
             Express.Response.status(
               Express.Response.StatusCode.BadRequest,
               res,
             )
             |> Express.Response.sendString(value)
           };
         Js.Promise.resolve(response);
       })
  );