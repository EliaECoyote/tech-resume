let genericErrorMessage = "generic process error encountered";
type jsEventData;
type std = {. [@bs.meth] "on": (string, string => unit) => unit};
type stdin = {. [@bs.meth] "end_": (string, string) => unit};

type eventData =
  | String(string)
  | Numeric(int)
  | Exn(Js.Exn.t);

let testEventData = (x: jsEventData): eventData =>
  switch (Js.typeof(x)) {
  | "string" => String(Obj.magic(x))
  | "number" => Numeric(Obj.magic(x))
  | "object" => Obj.magic(x)#message->Js.Exn.raiseError->Exn
  | _ => Js.Exn.raiseError({j|unexpected type: $x|j})
  };

// TODO: use [@bs.meth]
type process = {
  .
  [@bs.meth] "on": (string, jsEventData => unit) => unit,
  "stderr": std,
  "stdout": std,
  "stdin": stdin,
};

type spawnResult = Belt.Result.t(string, string);

[@bs.module "child_process"]
external spawn: (string, array(string)) => process = "spawn";
let spawn =
    (~path: string, ~args: array(string), ~src: string)
    : Js.Promise.t(spawnResult) => {
  let p = spawn(path, args);
  Js.Promise.make((~resolve, ~reject as _) => {
    p##on("error", error => {
      let message =
        switch (testEventData(error)) {
        | String(value) => value
        | Exn(value) =>
          Js.Exn.message(value)
          ->Belt.Option.getWithDefault(genericErrorMessage)
        | Numeric(value) => {j| error code: $value |j}
        };
      let output = message->Belt.Result.Error;
      resolve(. output);
    });
    let errorData = ref("");
    let outputData = ref("");
    p##stderr##on("data", d => errorData := errorData^ ++ d);
    p##stdout##on("data", d => outputData := outputData^ ++ d);
    p##on("close", code => {
      let errorMessage =
        switch (testEventData(code)) {
        | Numeric(0) => errorData^
        | _ => {j|process exited with code $code. $errorData|j}
        };
      let result =
        switch (errorMessage) {
        | "" => Belt.Result.Ok(outputData^)
        | _ => Belt.Result.Error(errorMessage)
        };
      resolve(. result);
    });
    %bs.raw
    {|p.stdin["end_"] = p.stdin.end|};
    p##stdin##end_(src, "utf8");
  });
};