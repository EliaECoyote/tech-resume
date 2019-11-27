module Styles = {
  open Css;

  let button = (colors: ThemeContext.colors) =>
    style([
      backgroundColor(colors.background),
      padding(`px(10)),
      border(`px(2), `solid, colors.accent),
      color(colors.primary),
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