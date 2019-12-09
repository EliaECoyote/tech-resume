module Styles = {
  open Css;

  let cta = (colors: ThemeContext.colors) =>
    style([
      textDecoration(`none),
      backgroundColor(colors.background),
      padding2(~v=`px(4), ~h=`px(10)),
      border(`px(2), `solid, colors.accent),
      fontSize(`rem(1.2)),
      textTransform(`uppercase),
      borderRadius(`px(4)),
      color(colors.primary),
      transition(~duration=250, "opacity"),
      hover([opacity(0.8)]),
      focus([opacity(0.6)]),
      disabled([opacity(0.3)]),
    ]);
};