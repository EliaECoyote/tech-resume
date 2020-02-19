module Styles = {
  open Css;
  let output =
    style([
      width(`percent(100.0)),
      height(`percent(100.0)),
      position(`relative),
    ]);

  let absoluteCanvasList = (_colors: ThemeContext.colors) =>
    style([
      backgroundColor(`hex("e3e7ed")),
      position(`absolute),
      top(`zero),
      bottom(`zero),
      left(`zero),
      right(`zero),
      overflow(`scroll),
    ]);

  let canvasContainer = style([padding(`px(20))]);

  let customBoxShadow =
    boxShadow(
      Shadow.box(~x=`zero, ~y=`zero, ~blur=`px(10), `rgba((0, 0, 0, 0.5))),
    );

  let canvas =
    style([
      customBoxShadow,
      width(`percent(100.0)),
      backgroundColor(Css_Colors.white),
    ]);

  let skeleton =
    style([
      paddingTop(`percent(141.0)),
      customBoxShadow,
      backgroundColor(`hex("eee")),
      unsafe(
        "backgroundImage",
        "linear-gradient(90deg, #eee, #d0d0d0, #eee)",
      ),
    ]);

  let fakePdf = style([paddingTop(`percent(141.0)), customBoxShadow]);
};

[@react.component]
let make = (~className=?, ~requestState, ()) => {
  let modelState = UsePDFModelList.hook(~pdfRequestState=requestState);
  let (theme, _) = React.useContext(ThemeContext.context);

  let outputContent =
    switch (modelState) {
    | Error => React.string("error")
    | Fetching => <SkeletonPulse className=Styles.skeleton />
    | Idle => React.null
    | Success(modelArray) =>
      modelArray |> Array.map(model => <Pdfpage model />) |> ReasonReact.array
    };

  <div className={CssHelpers.combine([Some(Styles.output), className])}>
    <div className={Styles.absoluteCanvasList(theme.colors)}>
      <div className=Styles.canvasContainer> outputContent </div>
    </div>
  </div>;
};