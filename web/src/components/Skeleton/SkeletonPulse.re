module Styles = {
  open Css;
  open Types.Color;
  open! Calc;

  let pulse =
    keyframes([
      (0, [backgroundPositions([`px(-200), `zero])]),
      (100, [backgroundPositions([`px(200) + `percent(100.0), `zero])]),
    ]);

  let skeletonPulse = (colors: ThemeContext.colors) =>
    style([
      display(`inlineBlock),
      height(`percent(100.0)),
      width(`percent(100.0)),
      borderRadius(`px(4)),
      backgroundSize(`size((`px(200), `percent(100.0)))),
      backgroundColor(colors.surface),
      backgroundRepeat(`noRepeat),
      unsafe(
        "backgroundImage",
        "linear-gradient(90deg,"
        ++ toString(colors.surface)
        ++ ","
        ++ toString(colors.skeletonAccentColor)
        ++ ","
        ++ toString(colors.surface)
        ++ ")",
      ),
      animationName(pulse),
      animationDuration(1200),
      animationTimingFunction(`easeInOut),
      animationIterationCount(`infinite),
    ]);
};

[@react.component]
let make = (~className=?) => {
  let (themeState, _) = React.useContext(ThemeContext.context);
  let className =
    Belt2.Css.combine([
      Some(Styles.skeletonPulse(themeState.colors)),
      className,
    ]);
  <div className />;
};