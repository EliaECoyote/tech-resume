let getNullable = nullableBuffer =>
  switch (Js.Null.toOption(nullableBuffer)) {
  | Some(value) => Js.log(value)
  | None => Js.log("no value")
  };

let runTest = () => {
  Js.log(("executing", {j|ls|j}));
  let spawnResult = Node.Child_process.spawnSync({j|ls|j});
  let result = Node.Child_process.readAs(spawnResult);
  let status = result##status;
  let outBuffer = Js.Null.toOption(result##stdout);
  let errBuffer = Js.Null.toOption(result##stderr);


  // let a: option(string) =
  //   switch (outBuffer) {
  //   | Some(value) => Some(Buffer.output_buffer() value)
  //   | None => None
  //   };
  
  // switch (result##stdout) {
  //   | Js.null => Js.log("ofiwaf")
  //   | Node.string_buffer(buffer) => Js.log("oanofa")
  // }
  // Node.Buffer.toString(stdout)
  // Js.log2("Exit code", result##status);
  // Js.log(("whole object result: ", result##stdout));
};