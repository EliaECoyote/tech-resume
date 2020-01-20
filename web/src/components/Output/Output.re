module Styles = {
  open Css;
  let output =
    style([
      width(`percent(100.0)),
      height(`percent(100.0)),
      padding(`px(20)),
    ]);
};

[@react.component]
let make = (~className=?, ~pdf=?, ()) => {
  let canvasRef = React.useRef(Js.Nullable.null);
  UseCanvasPDF.hook(~pdf, ~canvasRef);
  <div className={CssHelpers.combine([Some(Styles.output), className])}>
    <canvas ref={ReactDOMRe.Ref.domRef(canvasRef)} />
  </div>;
};