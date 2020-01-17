/**
 * converts a **TypedArray** to a PdfJS source element
 */
let toPDFjsSource = (data: Js.Typed_array.Int8Array.t) =>
  BsPdfjs.Document.Source.TypedArray({"data": data});