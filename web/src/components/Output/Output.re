module Styles = {
  open Css;
  let output = style([width(`percent(100.0)), height(`percent(100.0))]);
};

[@react.component]
let make = (~className=?, ~pdf=?, ()) => {
  React.useEffect1(
    () =>
      pdf
      |> Belt.Option.flatMap(_, pdf =>
           Wonka.fromValue(pdf)
           |> Wonka.tap((. value) => Js.log(("Buffer", value)))
           |> Wonka.map((. pdf) =>
                pdf
                // converts buffer to PDFjs source element
                |> PdfJSHelpers.toTypedArray
                |> PdfJSHelpers.toPDFjsSource
              )
           |> Wonka.tap((. value) => Js.log(("Pdf source", value)))
           |> Wonka.mergeMap((. source) =>
                source
                // uses PDFjs source to load the pdf data
                |> BsPdfjs.Global.getDocument(_, BsPdfjs.Global.inst)
                |> BsPdfjs.Global.DocumentLoadingTask.promise
                |> WonkaHelpers.fromPromise
              )
           |> Wonka.tap((. value) => Js.log(("Pdf loaded status", value)))
           |> Wonka.subscribe((. result) => Js.log(result))
           |> WonkaHelpers.getEffectCleanup
         ),
    [|pdf|],
  );
  <canvas
    className={CssHelpers.combine([Some(Styles.output), className])}
  />;
};

let default = make;