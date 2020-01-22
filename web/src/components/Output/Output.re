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
  let canvasRef = React.useRef(Js.Nullable.null);
  UseCanvasPDF.hook(~pdf, ~canvasRef);
  <div className={CssHelpers.combine([Some(Styles.output), className])}>
    <div className=Styles.absoluteCanvasList>
      <div className=Styles.canvasContainer>
        <canvas
          className=Styles.canvas
          ref={ReactDOMRe.Ref.domRef(canvasRef)}
        />
      </div>
    </div>
  </div>;
};