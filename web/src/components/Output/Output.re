module Styles = {
  open Css;
  let output = style([width(`percent(100.0)), height(`percent(100.0))]);
};

[@react.component]
let make = (~className=?, ~pdf=?, ()) => {
  let canvasRef = React.useRef(Js.Nullable.null);
  UseCanvasPDF.hook(~pdf, ~canvasRef);
  <canvas
    ref={ReactDOMRe.Ref.domRef(canvasRef)}
    className={CssHelpers.combine([Some(Styles.output), className])}
  />;
};