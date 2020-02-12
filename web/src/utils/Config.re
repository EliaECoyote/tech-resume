[@bs.val] external window: _ = "window";
let isBrowser = Js.typeof(window) != "undefined";

[@bs.val] external apiScheme: string = "process.env.GATSBY_API_SCHEME";

[@bs.val] external apiHost: string = "process.env.GATSBY_API_HOST";