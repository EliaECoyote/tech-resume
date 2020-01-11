let generatePDFjsSource = (data: Js.Typed_array.Int8Array.t) =>
  BsPdfjs.Document.Source.TypedArray({"data": data});

external toTypedArray: Fetch.arrayBuffer => Js.Typed_array.Int8Array.t =
  "%identity";
let toTypedArray = arrayBuffer =>
  arrayBuffer |> toTypedArray |> generatePDFjsSource;