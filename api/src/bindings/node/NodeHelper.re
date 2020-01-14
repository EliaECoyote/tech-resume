module StringBuffer = {
  type t; /* can be either string or buffer */
  type stringBufferValue = [ | `Str(Js.String.t) | `Buf(Node.Buffer.t)];
  /** We except a good inliner will eliminate such boxing in the future */
  let getValue = (x: t): stringBufferValue =>
    Js.typeof(x) == "string" ? `Str(Obj.magic(x)) : `Buf(Obj.magic(x));

  let getStringValue = (x: t): string =>
    switch (getValue(x)) {
    | `Str(value) => value
    | `Buf(value) => Node.Buffer.toString(value)
    };
};
module Buffer = {
  type t = Node.Buffer.t;
  [@bs.send] external toString: (t, ~encoding: string) => string = "toString";
};