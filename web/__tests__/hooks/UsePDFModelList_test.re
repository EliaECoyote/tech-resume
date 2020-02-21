open Jest;

module TestData = {
  [@bs.val] external dirname: string = "__dirname";
  external toTypedArray: Node.Buffer.t => Js_typed_array.Int8Array.t =
    "%identity";
  let invalidPdfSource =
    Js_typed_array.Int8Array.make([|1, 2, 3|]) |> Belt2.PdfJs.toPdfJsSource;
  let validPdfSource =
    Node.Fs.readFileSync("__tests__/sample.pdf", `binary)
    |> Node.Buffer.fromStringWithEncoding(_, `binary)
    |> toTypedArray
    |> Belt2.PdfJs.toPdfJsSource;
};

beforeAll(
  [%bs.raw
    {|
     () => {
       const pdfjsWorker = require("pdfjs-dist/build/pdf.worker.entry");
       const pdfjs = require("pdfjs-dist/build/pdf");
       pdfjs.GlobalWorkerOptions.workerSrc = pdfjsWorker;
     }
   |}
  ],
);

afterAll(
  [%bs.raw
    {|
     () => {
       const pdfjs = require("pdfjs-dist/build/pdf");
       pdfjs.GlobalWorkerOptions.workerSrc = undefined;
     }
   |}
  ],
);

describe("pdf document loading", () => {
  testAsync(
    "should fail with invalid source",
    ~timeout=100,
    done_ => {
      let _ =
        XWonka.fromValue(TestData.invalidPdfSource)
        |> XWonka.switchMap(UsePDFModelList.PDFLoader.loadDocument)
        |> XWonka.subscribe(document => {
             let assertion =
               Belt.Result.isOk(document)
                 ? fail("source should be invalid") : pass;
             done_(assertion);
           });
      ();
    },
  );
  testAsync(
    "should succeed with valid source",
    ~timeout=100,
    done_ => {
      let _ =
        XWonka.fromValue(TestData.validPdfSource)
        |> XWonka.switchMap(UsePDFModelList.PDFLoader.loadDocument)
        |> XWonka.subscribe(document => {
             let assertion =
               switch (document) {
               | Belt.Result.Ok(_value) => pass
               | Belt.Result.Error(error) =>
                 Js.Console.error(error);
                 Js.Exn.message(error)
                 |> Belt.Option.getWithDefault(_, "source should be valid")
                 |> fail;
               };
             done_(assertion);
           });
      ();
    },
  );
});