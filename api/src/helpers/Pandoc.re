let run = (~fromPath: string, ~toPath: string) => {
  let args = {j|-f $fromPath -t $toPath|j};
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