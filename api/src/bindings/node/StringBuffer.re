type stringBuffer; /* can be either string or buffer */
type t = stringBuffer;

type stringBufferValue = [ | `Str(Js.String.t) | `Buf(Node.Buffer.t)];

/** We except a good inliner will eliminate such boxing in the future */
let getValue = (x: stringBuffer): stringBufferValue =>
  Js.typeof(x) == "string" ? `Str(Obj.magic(x)) : `Buf(Obj.magic(x));

let getStringValue = (x: stringBuffer): string =>
  switch (getValue(x)) {
  | `Str(value) => value
  | `Buf(value) => Node.Buffer.toString(value)
  };