module Styles = {
  open Css;
  let output = style([width(`percent(100.0)), height(`percent(100.0))]);
};

[@react.component]
let make = (~className=?, ~pdf: Fetch.arrayBuffer=?) => {
  React.useEffect1(
    () => 
      // Wonka.fromValue(pdf)
      // |> Wonka.map((. pdf) => PDF);

      // let url: BsPdfjs.Document.Source.t = {
      //   data: ,
      // };
      // BsPdfjs.Global.getDocument;
      // let loadingTask =
      // var loadingTask = pdfjsLib.getDocument({data: pdfData});
      None
    ,
    [|pdf|],
  );
  <canvas
    className={CssHelpers.combine([Some(Styles.output), className])}
  />;
};

let default = make;