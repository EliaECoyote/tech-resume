module Styles = {
  open Css;

  let button = (colors: ThemeContext.colors) =>
    style([
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

type buttonKind =
  | StandardButton
  | LinkButton(string);

let getButtonKind = (~url=?, ()) =>
  switch (url) {
  | Some(value) => LinkButton(value)
  | None => StandardButton
  };

[@react.component]
let make =
  React.forwardRef((~children, ~disabled=?, ~onClick=?, ~url=?, buttonRef) => {
    let (theme, _) = React.useContext(ThemeContext.context);
    let kind = getButtonKind(~url?, ());
    switch (kind) {
    | StandardButton =>
      <button
        ?onClick
        ?disabled
        className={Styles.button(theme.colors)}
        ref=?{
          buttonRef
          ->Js.Nullable.toOption
          ->Belt.Option.map(ReactDOMRe.Ref.domRef)
        }>
        children
      </button>
    | LinkButton(value) =>
      <a
        href=value
        ?onClick
        ?disabled
        className={Styles.button(theme.colors)}
        ref=?{
          buttonRef
          ->Js.Nullable.toOption
          ->Belt.Option.map(ReactDOMRe.Ref.domRef)
        }>
        children
      </a>
    };
  });