// child_process further documentation: https://nodejs.org/api/child_process.html#child_process_class_childprocess
type spawnResult =
  | Ok(string)
  | Error(string);

[@bs.deriving abstract]
type childProcess;
[@bs.send] [@bs.scope "stdout"]
external onStdout: (childProcess, [@bs.as "data"] _, string => unit) => unit =
  "on";
[@bs.send] [@bs.scope "stderr"]
external onStderr: (childProcess, [@bs.as "data"] _, string => unit) => unit =
  "on";
[@bs.send] [@bs.scope "stdin"]
external endStdin: (childProcess, string, string) => unit = "end";
[@bs.send]
external on:
  (
    childProcess,
    [@bs.string] [
      | `line(string => unit)
      | `error(Js.Exn.t => unit)
      | `disconnect(unit => unit)
      | `exit((int, string) => unit)
      | `close((int, string) => unit)
    ]
  ) =>
  unit =
  "on";

type t = childProcess;

[@bs.module "child_process"]
external spawn: (string, array(string)) => childProcess = "spawn";
let spawn =
    (~path: string, ~args: array(string), ~src: string)
    : Js.Promise.t(spawnResult) => {
  let p = spawn(path, args);
  Js.Promise.make((~resolve, ~reject as _) => {
    let outputData = ref("");
    let errorData = ref("");
    p->on(
      `error(
        err => {
          let errorMessage =
            Js.Exn.message(err)
            ->Belt.Option.getWithDefault("generic process error")
            ->Error;
          resolve(. errorMessage);
        },
      ),
    );
    p->on(
      `close(
        (code, _) => {
          let output =
            switch (code) {
            | 0 => Ok(outputData^)
            | _ => Error(errorData^)
            };
          resolve(. output);
        },
      ),
    );
    onStdout(p, data => outputData := outputData^ ++ data);
    onStderr(p, data => errorData := errorData^ ++ data);
    endStdin(p, src, "utf8");
  });
};