let getNullable = nullableBuffer =>
  switch (Js.Null.toOption(nullableBuffer)) {
  | Some(value) => Js.log(value)
  | None => Js.log("no value")
  };

let runTest = () => {
  Js.log(("executing", "ls"));
  let spawnResult = Node.Child_process.spawnSync("ls");
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