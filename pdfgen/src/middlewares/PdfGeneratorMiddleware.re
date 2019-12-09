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

let deleteFileIfExists = fileName =>
  NodeFs.wonkaStat(~path=fileName, ~options=None, ())
  |> Wonka.mergeMap((. value) =>
       switch (value) {
       | Belt.Result.Ok(_) =>
         NodeFs.wonkaUnlink(~path=fileName) |> Wonka.map((. _) => ())
       | Belt.Result.Error(_) => Wonka.fromValue()
       }
     );

let generatePdf = (html: string) => {
  let pdfFileName = generateFileName(Pdf);
  ChildProcess.spawn(
    ~path="wkhtmltopdf",
    ~args=[|"-", pdfFileName|],
    ~src=html,
  )
  |> Wonka.fromPromise
  |> Wonka.map((. result) =>
       switch (result) {
       | ChildProcess.Ok(_) =>
         Node.Path.resolve(".", pdfFileName) |> (path => Ok(path))
       | ChildProcess.Error(value) => Error(value)
       }
     );
  // |> Wonka.mergeMap((. value) =>
  //      deleteFileIfExists(pdfFileName) |> Wonka.map((. _) => value)
  //    );
};

let middleware =
  Express.PromiseMiddleware.from((_, req, res) =>
    Express.Request.query(req)
    |> Wonka.fromValue
    |> Wonka.map((. query) =>
         Js.Dict.get(query, "html")
         ->Belt.Option.flatMap(Js.Json.decodeString)
         ->Belt.Option.getWithDefault("")
       )
    |> Wonka.mergeMap((. html) => generatePdf(html))
    |> Wonka.toPromise
    |> Js.Promise.then_(result => {
         let response =
           switch (result) {
           | Ok(value) =>
             Express.Response.status(Express.Response.StatusCode.Ok, res)
             |> ExpressBs.download(~path=value, ~filename="resume.pdf")
           | Error(value) =>
             Js.log(value);
             Express.Response.status(
               Express.Response.StatusCode.BadRequest,
               res,
             )
             |> Express.Response.sendString("pdf generation failed");
           };
         Js.Promise.resolve(response);
       })
  );