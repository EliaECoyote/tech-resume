type size =
  | Small
  | Large;

module Styles = {
  open Css;

  let cta = (colors: ThemeContext.colors, ~size: size=Large, ()) =>
    style([
      textDecoration(`none),
      backgroundColor(colors.background),
      border(`px(2), `solid, colors.accent),
      textTransform(`uppercase),
      borderRadius(`px(4)),
      color(colors.primary),
      transition(~duration=250, "opacity"),
      hover([opacity(0.8)]),
      focus([opacity(0.6)]),
      disabled([opacity(0.3)]),
      switch (size) {
      | Small => fontSize(`rem(0.8))
      | Large => fontSize(`rem(1.2))
      },
      switch (size) {
      | Small => padding2(~v=`px(2), ~h=`px(4))
      | Large => padding2(~v=`px(4), ~h=`px(10))
      },
    ]);
};