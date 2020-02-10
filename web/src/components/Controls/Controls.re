type size =
  | Small
  | Large;

module Styles = {
  open Css;

  let disabledRules = [opacity(0.3)];

  let cta = (colors: ThemeContext.colors, ~size: size=Large, ()) =>
    style([
      textDecoration(`none),
      backgroundColor(colors.primary),
      border(`px(0), `none, `transparent),
      textTransform(`uppercase),
      borderRadius(`px(4)),
      color(colors.secondary),
      transition(~duration=250, "opacity"),
      cursor(`pointer),
      hover([opacity(0.8)]),
      focus([opacity(0.6)]),
      fontFamily("'Montserrat', sans-serif"),
      disabled(disabledRules),
      letterSpacing(`initial),
      switch (size) {
      | Small => fontSize(`rem(0.6))
      | Large => fontSize(`rem(1.0))
      },
      switch (size) {
      | Small => padding2(~v=`px(1), ~h=`px(2))
      | Large => padding2(~v=`px(4), ~h=`px(8))
      },
    ]);
};