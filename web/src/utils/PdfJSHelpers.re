external toTypedArray: Fetch.arrayBuffer => Js.Typed_array.Int8Array.t =
  "%identity";

/**
 * converts a **TypedArray** to a PdfJS source element
 */
let toPDFjsSource = (data: Js.Typed_array.Int8Array.t) =>
  BsPdfjs.Document.Source.TypedArray({"data": data});

/**
 * converts fetch arrayBuffer to an Int8Array **TypedArray**
 */
let toTypedArray = arrayBuffer => arrayBuffer |> toTypedArray;