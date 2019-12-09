type fileNameKind =
  | Html
  | Pdf;

/**
 * generates a random pdf / html filename
 */
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

/**
 * checks if a file exists before deleting it from
 * the file system
 */
let deleteFileIfExists = path =>
  NodeFs.wonkaStat(~path, ~options=None, ())
  |> Wonka.mergeMap((. value) =>
       switch (value) {
       | Belt.Result.Ok(_) =>
         NodeFs.wonkaUnlink(~path) |> Wonka.map((. _) => ())
       | Belt.Result.Error(_) => Wonka.fromValue()
       }
     );

/**
 * generates a pdf file, given an html string, by spawning
 * a wkhtmltopdf process.
 * returns the generated pdf path when successful
 */
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
         Node.Path.resolve(".", pdfFileName)
         |> (path => Belt.Result.Ok(path))
       | ChildProcess.Error(value) => Belt.Result.Error(value)
       }
     );
};

let middleware =
  Express.PromiseMiddleware.from((_, req, res) =>
    Express.Request.query(req)
    |> Wonka.fromValue
    |> Wonka.map((. query)
         // grabs the html string from request query string
         =>
           Js.Dict.get(query, "html")
           ->Belt.Option.flatMap(Js.Json.decodeString)
           ->Belt.Option.getWithDefault("")
         )
    |> Wonka.mergeMap((. html)
         // generates the pdf
         => generatePdf(html))
    |> Wonka.mergeMap((. pathResult) => {
         // sends the pdf file, if available
         let response =
           switch (pathResult) {
           | Belt.Result.Ok(value) =>
             res
             |> Express.Response.status(Express.Response.StatusCode.Ok)
             |> ExpressBs.download(~path=value, ~filename="resume.pdf")
           | Belt.Result.Error(value) =>
             Js.log(value);
             res
             |> Express.Response.status(
                  Express.Response.StatusCode.BadRequest,
                )
             |> Express.Response.sendString("pdf generation failed");
           };
         Wonka.fromValue((response, pathResult));
       })
    |> Wonka.mergeMap((. (response, pathResult))
         // deletes the generated pdf file
         =>
           pathResult
           ->Belt.Result.map(deleteFileIfExists)
           ->Belt.Result.getWithDefault(Wonka.fromValue())
           |> Wonka.map((. _) => response)
         )
    |> Wonka.toPromise
  );