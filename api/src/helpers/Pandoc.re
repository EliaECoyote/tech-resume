type fileKind =
  | Markdown
  | Html
  | Pdf

let getFileKindValue = kind =>
  switch (kind) {
    | Markdown => "markdown"
    | Html => "html"
    | Pdf => "pdf"
  }

let run = (~src: string, ~fromKind: fileKind, ~toKind: fileKind) => {
  let fromKindValue = getFileKindValue(fromKind)
  let toKindValue = getFileKindValue(toKind)
  let args = {j|-f $fromKindValue -t $toKindValue|j};
  Js.log(("executing", {j|pandoc $args|j}));
  let spawnResult = Node.Child_process.spawnSync({j|pandoc $args|j});
  let result = Node.Child_process.readAs(spawnResult);
  let data =
    Js.Null.toOption(result##stdout)
    ->Belt.Option.flatMap(value =>
        value
        ->CustomPervasives.buffer_of_string_buffer
        ->Node.Buffer.toString
        ->Some
      );
  Js.log(data);
};