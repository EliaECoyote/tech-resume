[@bs.new] external makeError: string => Js.Exn.t = "Error";

external promiseExnToJsExn: Js.Promise.error => Js.Exn.t = "%identity";