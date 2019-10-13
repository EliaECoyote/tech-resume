type fileKind =
  | Markdown
  | Html
  | Pdf;

let getFileKindValue = kind =>
  switch (kind) {
  | Markdown => "markdown"
  | Html => "html"
  | Pdf => "pdf"
  };

// let getBufferStringOutput = bufferString =>
//   Js.Null.toOption(bufferString)
//   ->Belt.Option.flatMap(value =>
//       value
//       ->CustomPervasives.buffer_of_string_buffer
//       ->Node.Buffer.toString
//       ->Some
//     );

// let getProcessResultOutput = result => {
//   Js.log2(result##stdout, result##stderr);
//   let data = getBufferStringOutput(result##stdout);
//   let error = getBufferStringOutput(result##stderr);
//   switch (data) {
//   | Some(value) => Result(value)
//   | None =>
//     switch (error) {
//     | Some(value) => Error(value)
//     | None => Error("something went wrong")
//     }
//   };
// };

let cmd = "/usr/local/bin/pandoc";

// let test = () => {
//   Js.log("\n\nexecuting test:\n");
//   let encoding = Node.Child_process.option(~cwd=".", ~encoding="utf8", ());
//   let testExecSync = Node.Child_process.execSync(cmd, encoding);
//   Js.log2(testExecSync, "\ntest completed \n\n");
// };

let run =
    (~src: string, ~fromKind: fileKind, ~toKind: fileKind)
    : Js.Promise.t(ChildProcess.result) => {
  let fromValue = getFileKindValue(fromKind);
  let toValue = getFileKindValue(toKind);
  let args = [|"-f", fromValue, "-t", toValue|];
  Js.log(("spawning process", cmd, args, src));
  ChildProcess.spawn(~path="pandoc", ~args, ~src);
};