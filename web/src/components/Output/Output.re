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

  let skeleton = style([paddingTop(`percent(141.0)), customBoxShadow]);

  let fakePdf = style([paddingTop(`percent(141.0)), customBoxShadow]);
};

let getPdf = requestState =>
  switch (requestState) {
  | AsyncTask.Success(data) => Some(data)
  | _ => None
  };

[@react.component]
let make = (~className=?, ~requestState, ()) => {
  open UsePDFModelList.Types;
  let modelState = UsePDFModelList.hook(~pdf=getPdf(requestState));
  let (theme, _) = React.useContext(ThemeContext.context);

  let outputContent =
    switch (requestState, modelState) {
    | (Fetching, _)
    | (_, Fetching) => <SkeletonPulse className=Styles.skeleton />
    | (Idle, _)
    | (_, Idle) => <SkeletonPulse className=Styles.skeleton />
    | (Error, _)
    | (_, Error) => React.string @@ "error"
    | (Success(_), Success(modelArray)) =>
      modelArray |> Array.map(model => <Pdfpage model />) |> ReasonReact.array
    };

  <div className={CssHelpers.combine([Some(Styles.output), className])}>
    <div className={Styles.absoluteCanvasList(theme.colors)}>
      <div className=Styles.canvasContainer> outputContent </div>
    </div>
  </div>;
};