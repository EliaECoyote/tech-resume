module Styles = {
  open Css;

  let editorContainer = (colors: ThemeContext.colors) =>
    style([padding(`px(10)), border(`px(2), `solid, colors.accent)]);

  let editor = style([height(`vh(80.0))]);
};

[@react.component]
let make =
  React.forwardRef((~children, ~onClick, buttonRef) => {
    let (state, _) = React.useContext(ThemeContext.context);
    <button
      onClick
      ref=?{
        buttonRef
        ->Js.Nullable.toOption
        ->Belt.Option.map(ReactDOMRe.Ref.domRef)
      }>
      children
    </button>;
  });