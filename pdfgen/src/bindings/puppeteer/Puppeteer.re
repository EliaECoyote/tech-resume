/** Configuration options for [connect]. */
type connectOptions = {
  browserWSEndpoint: Js.nullable(string),
  ignoreHTTPSErrors: Js.nullable(bool),
  defaultViewport: Js.nullable(Viewport.t),
  slowMo: Js.nullable(float),
};

/** Attach Puppeteer to an existing Chromium instance. */
[@bs.val] [@bs.module "puppeteer"]
external connect:
  (~options: connectOptions=?, unit) => Js.Promise.t(Browser.t) =
  "connect";

/** Path where Puppeteer expects to find bundled Chromium. */
[@bs.val] [@bs.module "puppeteer"]
external executablePath: unit => string = "executablePath";

module IgnoreDefaultArgs = {
  type t;
  type arg =
    | Bool(bool)
    | Args(array(string));

  let encode: arg => t =
    fun
    | Bool(b) => Obj.magic(b)
    | Args(args) => Obj.magic(args);

  let encodeOpt = Belt.Option.map(_, encode);
};

type launchOptions = {
  ignoreHTTPSErrors: option(bool),
  headless: option(bool),
  executablePath: option(string),
  slowMo: option(float),
  defaultViewport: Js.nullable(Viewport.t),
  args: option(array(string)),
  ignoreDefaultArgs: option(IgnoreDefaultArgs.t),
  handleSIGINT: option(bool),
  handleSIGTERM: option(bool),
  handleSIGHUP: option(bool),
  timeout: option(int),
  dumpio: option(bool),
  userDataDir: option(string),
  env: option(Js.Dict.t(string)),
  devtools: option(bool),
  pipe: option(bool),
};

/** Launch a browser instance. */
[@bs.val] [@bs.module "puppeteer"]
external launch: (~options: launchOptions=?, unit) => Js.Promise.t(Browser.t) =
  "launch";

module DefaultArgsOptions = {
  type t = {
    headless: option(bool),
    args: option(array(string)),
    userDataDir: option(string),
    devtools: option(bool),
  };
};

/** The default flags that Chromium will launch with. */
[@bs.val] [@bs.module "puppeteer"]
external defaultArgs:
  (~options: DefaultArgsOptions.t=?, unit) => array(string) =
  "defaultArgs";

type browserFetcherOptions = {
  .
  host: option(string),
  path: option(string),
  platform: option(string),
};

[@bs.obj]
external makeBrowserFetcherOptions:
  (~host: string=?, ~path: string=?, ~platform: string=?, unit) =>
  browserFetcherOptions =
  "";

let makeBrowserFetcherOptions =
    (~host=?, ~path=?, ~platform=?, ()): browserFetcherOptions =>
  makeBrowserFetcherOptions(
    ~host?,
    ~path?,
    ~platform=?
      platform |> Js.Option.map((. v) => v |> BrowserFetcher.platformToJs),
    (),
  );

[@bs.val] [@bs.module "puppeteer"]
external createBrowserFetcher:
  (~options: browserFetcherOptions=?, unit) => BrowserFetcher.t =
  "createBrowserFetcher";