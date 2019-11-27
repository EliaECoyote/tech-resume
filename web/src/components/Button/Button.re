module Styles = {
  open Css;

  let button = (colors: ThemeContext.colors) =>
    style([
      backgroundColor(colors.background),
      padding2(~v=`px(4), ~h=`px(10)),
      border(`px(2), `solid, colors.accent),
      fontSize(`rem(1.2)),
      borderRadius(`px(4)),
      color(colors.primary),
      transition(~duration=300, "opacity"),
      hover([opacity(0.8)]),
    ]);
};

[@react.component]
let make =
  React.forwardRef((~children, ~onClick, buttonRef) => {
    let (state, _) = React.useContext(ThemeContext.context);
    <button
      onClick
      className={Styles.button(state.colors)}
      ref=?{
        buttonRef
        ->Js.Nullable.toOption
        ->Belt.Option.map(ReactDOMRe.Ref.domRef)
      }>
      children
    </button>;
  });