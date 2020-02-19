/**
 * converts a **TypedArray** to a PdfJS source element
 */
let toPdfJsSource = (data: Js.Typed_array.Int8Array.t) =>
  BsPdfjs.Document.Source.TypedArray({"data": data});

/**
 * sets the Pdf JS worker.
 */
let setPdfJsWorker = () =>
  // TODO: update this to use dynamic imports, as described in
  // https://github.com/mozilla/pdf.js/issues/10478#issuecomment-518673665
  BsPdfjs.Global.setWorkerSrc(
    BsPdfjs.Global.inst,
    "//cdnjs.cloudflare.com/ajax/libs/pdf.js/2.2.228/pdf.worker.js",
  );