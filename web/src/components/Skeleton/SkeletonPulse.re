module Styles = {
  open Css;

  let pulse =
    Calc.(
      keyframes([
        (0, [backgroundPositions([`px(-200), `zero])]),
        (100, [backgroundPositions([`px(200) + `percent(100.0), `zero])]),
      ])
    );

  let skeletonPulse =
    style([
      display(`inlineBlock),
      height(`percent(100.0)),
      width(`percent(100.0)),
      borderRadius(`px(4)),
      backgroundSize(`size((`px(200), `percent(100.0)))),
      backgroundColor(`hex("eee")),
      backgroundRepeat(`noRepeat),
      unsafe(
        "backgroundImage",
        "linear-gradient(90deg, #eee, #d0d0d0, #eee)",
      ),
      animationName(pulse),
      animationDuration(1200),
      animationTimingFunction(`easeInOut),
      animationIterationCount(`infinite),
    ]);
};

[@react.component]
let make = (~className=?, ~background=?) => {
  let className =
    CssHelpers.combine([Some(Styles.skeletonPulse), className]);
  let style = ReactDOMRe.Style.make(~backgroundColor=?background, ());
  <div className style />;
};