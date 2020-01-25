module Styles = {
  open Css;
  let canvas =
    style([width(`percent(100.0)), backgroundColor(Css_Colors.white)]);
};

[@react.component]
let make =
    (~model: Belt.Result.t(UsePDFModelList.Types.pdfValidModel, string)) => {
  let _ = UsePDFModelRenderer.hook(~model);
  switch (model) {
  | Belt.Result.Ok(model) =>
    <canvas
      id={model.id}
      key={model.id}
      height={Js.Float.toString(model.logicalHeight)}
      width={Js.Float.toString(model.logicalWidth)}
      className=Styles.canvas
    />
  | Belt.Result.Error(message) => <h1> {React.string @@ message} </h1>
  };
};