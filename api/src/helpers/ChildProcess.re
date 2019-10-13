type std = {. on: (string, string => unit) => unit};
type stdin = {. end_: (string, string) => unit};
// TODO: use [@bs.meth]
type process = {
  .
  on: (string, Js.Exn.t => unit) => unit,
  stderr: std,
  stdout: std,
  stdin: stdin,
};

type spawnResult = Js.Promise.t(Belt.Result.t(string, string));

[@bs.module "child_process"]
external spawn: (string, array(string)) => process = "spawn";
let spawn = (~path: string, ~args: array(string), ~src: string): spawnResult => {
  let p = spawn(path, args);
  Js.Promise.make((~resolve, ~reject as _) => {
    p#on("error", error => {
      let output =
        Js.Exn.message(error)
        ->Belt.Option.getWithDefault("generic process error encountered")
        ->Belt.Result.Error;
      resolve(. output);
    });
    let errorData = ref("");
    let outputData = ref("");
    p#stderr#on("data", d => errorData := errorData^ ++ d);
    p#stdout#on("data", d => outputData := outputData^ ++ d);
    p#on("close", code => {
      let errorMessage =
        switch (code) {
        // | 0 => errorData^
        | _ => {j|process exited with code $code. $errorData^|j}
        };
      let result =
        switch (errorMessage) {
        | "" => Belt.Result.Ok(outputData^)
        | _ => Belt.Result.Error(errorMessage)
        };
      resolve(. result);
    });
    p#stdin#end_(src, "utf8");
  });
};