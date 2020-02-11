type blobT;

type t = blobT;

type makeBlobOptionsT = {
  [@bs.as "type"]
  type_: option(string),
  endings: option(string),
};

[@bs.new]
external makeFromArrayBuffer:
  (array(Js.Typed_array.ArrayBuffer.t), ~options: makeBlobOptionsT=?, unit) =>
  t =
  "Blob";

[@bs.new]
external makeFromInt8Array:
  (array(Js.Typed_array.Int8Array.t), ~options: makeBlobOptionsT=?, unit) => t =
  "Blob";