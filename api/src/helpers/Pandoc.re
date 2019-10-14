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

let cmd = "/usr/local/bin/pandoc";

let run =
    (~src: string, ~fromKind: fileKind, ~toKind: fileKind)
    : Js.Promise.t(ChildProcess.spawnResult) => {
  let fromValue = getFileKindValue(fromKind);
  let toValue = getFileKindValue(toKind);
  let args = [|"-f", fromValue, "-t", toValue|];
  Js.log(("spawning process", cmd, args, src));
  ChildProcess.spawn(~path="pandoc", ~args, ~src);
};