// custom puppeteer options suggested by the
// official docs for alpine-based docker images
// https://github.com/puppeteer/puppeteer/blob/master/docs/troubleshooting.md#running-on-alpine
let launchOptions: Puppeteer.launchOptions = {
  ignoreHTTPSErrors: None,
  headless: None,
  executablePath: Some("/usr/bin/chromium-browser"),
  slowMo: None,
  defaultViewport: Js.Nullable.null,
  args: Some([|"--disable-dev-shm-usage"|]),
  ignoreDefaultArgs: None,
  handleSIGINT: None,
  handleSIGTERM: None,
  handleSIGHUP: None,
  timeout: None,
  dumpio: None,
  userDataDir: None,
  env: None,
  devtools: None,
  pipe: None,
};

let margin: Page.margin = {
  top: Some("30"),
  bottom: Some("30"),
  left: Some("50"),
  right: Some("50"),
};

let pdfOptions: Page.pdfOptions = {
  path: None,
  scale: None,
  displayHeaderFooter: None,
  headerTemplate: None,
  footerTemplate: None,
  printBackground: Some(true),
  landscape: None,
  pageRanges: None,
  format: None,
  width: None,
  height: None,
  margin: Some(margin),
  preferCSSPageSize: Some(true),
};

let generatePdf = html =>
  Puppeteer.launch(~options=launchOptions, ())
  |> Js.Promise.then_(browser =>
       browser
       |> Browser.newPage
       |> Js.Promise.then_(page => Js.Promise.resolve((browser, page)))
     )
  |> Js.Promise.then_(((browser, page)) =>
       page
       |> Page.setContent(_, html)
       |> Js.Promise.then_(() => Js.Promise.resolve((browser, page)))
     )
  |> Js.Promise.then_(((browser, page)) =>
       page
       |> Page.pdf(_, pdfOptions)
       |> Js.Promise.then_(pdfBuffer =>
            Js.Promise.resolve((browser, pdfBuffer))
          )
     )
  |> Js.Promise.then_(((browser, pdfBuffer)) =>
       browser
       |> Browser.close
       |> Js.Promise.then_(() => Js.Promise.resolve(pdfBuffer))
     )
  |> WonkaHelpers.fromPromise;

let middleware =
  Express.PromiseMiddleware.from((_, req, res) =>
    Express.Request.bodyText(req)
    |> Wonka.fromValue
    |> Wonka.map((. body) => Belt.Option.getWithDefault(body, ""))
    |> Wonka.mergeMap((. html)
         // generates the pdf
         => generatePdf(html))
    |> Wonka.mergeMap((. result) => {
         // sends the pdf file, if available
         let response =
           switch (result) {
           | Belt.Result.Ok(pdfBuffer) =>
             res
             |> Express.Response.status(Express.Response.StatusCode.Ok)
             |> Express.Response.sendBuffer(pdfBuffer)
           | Belt.Result.Error(value) =>
             Js.Console.error(value);
             res
             |> Express.Response.status(
                  Express.Response.StatusCode.BadRequest,
                )
             |> Express.Response.sendString("[pdfgen] pdf generation failed");
           };
         Wonka.fromValue(response);
       })
    |> Wonka.toPromise
  );