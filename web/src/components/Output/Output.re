module Styles = {
  open Css;
  let output =
    style([
      width(`percent(100.0)),
      height(`percent(100.0)),
      position(`relative),
    ]);
  let absoluteCanvasList =
    style([
      position(`absolute),
      top(`zero),
      bottom(`zero),
      left(`zero),
      right(`zero),
      overflow(`scroll),
    ]);
  let canvasContainer = style([padding(`px(20))]);
  let canvas =
    style([width(`percent(100.0)), backgroundColor(Css_Colors.white)]);
};

[@react.component]
let make = (~className=?, ~pdf=?, ()) => {
  open UsePDFModelList.Types;
  let modelState = UsePDFModelList.hook(~pdf);

  let outputContent =
    switch (modelState) {
    | Idle => <h1> {React.string @@ "idle"} </h1>
    | Fetching => <h1> {React.string @@ "fetching"} </h1>
    | Error => <h1> {React.string @@ "error"} </h1>
    | Success(modelArray) =>
      modelArray |> Array.map(model => <Pdfpage model />) |> ReasonReact.array
    };

  <div className={CssHelpers.combine([Some(Styles.output), className])}>
    <div className=Styles.absoluteCanvasList>
      <div className=Styles.canvasContainer> outputContent </div>
    </div>
  </div>;
};