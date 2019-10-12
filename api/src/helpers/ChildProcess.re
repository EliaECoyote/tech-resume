type embeddedProcess = {
  .
  onError: (Js.Exn.t => unit) => unit,
  onErrorData: (string => unit) => unit,
  onOutputData: (string => unit) => unit,
  onClose: (int => unit) => unit,
  endStdin: (string, string) => unit,
};

type result = Belt.Result.t(string, string);

let nodeSpawn: (string, array(string)) => embeddedProcess = [%bs.raw
  {|
  function (path, args) {
    var spawn = require('child_process').spawn
    var process = spawn(path, args)
    return {
      onError: function(errorHandler) { process.on('error', errorHandler) },
      onErrorData: function(errorDataHandler) { process.stderr.on('data', errorDataHandler) },
      onOutputData: function(outputDataHandler) { process.stdout.on('data', errorDataHandler) },
      onClose: function(onCloseHandler) {process.on('close', onCloseHandler)},
      endStdin: function(src) { process.stdin.end(src, encoding) }
    }
  }
|}
];

let spawn =
    (~path: string, ~args: array(string), ~src: string): Js.Promise.t(result) =>
  Js.Promise.make((~resolve, ~reject as _) => {
    let process: embeddedProcess = nodeSpawn(path, args);
    process#onError(error => {
      let output =
        Js.Exn.message(error)
        ->Belt.Option.getWithDefault("generic process error encountered")
        ->Belt.Result.Error;
      resolve(. output);
    });
    let outputData = ref("");
    let errorData = ref("");
    process#onErrorData(d => errorData := errorData^ ++ d);
    process#onOutputData(d => outputData := outputData^ ++ d);
    process#onClose(code => {
      let errorMessage =
        switch (code) {
        | 0 => errorData^
        | _ => {j|process exited with code $code. $errorData^|j}
        };
      let result =
        switch (errorMessage) {
        | "" => Belt.Result.Ok(outputData^)
        | _ => Belt.Result.Error(errorMessage)
        };
      resolve(. result);
    });
    process#endStdin(src, "utf8");
  });