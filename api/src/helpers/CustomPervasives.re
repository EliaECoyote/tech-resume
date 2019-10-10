type outputKind =
  | StringOutput(string)
  | BufferOutput(Node.Buffer.t);

let getStringBufferOutput = (x: Node.string_buffer): outputKind =>
  switch (Js.typeof(x))
  | "string" => StringOutput(Obj.magic(x));
  | _ => BufferOutput(Obj.magic(x))


let buffer_of_string_buffer = sb =>
  switch (getStringBufferOutput(sb)) {
  | StringOutput(value) => Node.Buffer.fromStringWithEncoding(value, `utf8)
  | BufferOutput(value) => value
  };