module Styles = {
  open Css;

  let disabledRules = [opacity(0.3)];

  let link =
      (colors: ThemeContext.colors, ~size=Controls.Large, ~disabled=false, ()) =>
    merge([
      Controls.Styles.cta(colors, ~size, ()),
      style(disabled ? Controls.Styles.disabledRules : []),
    ]);
};

[@react.component]
let make =
  React.forwardRef(
    (
      ~children,
      ~size=?,
      ~disabled=?,
      ~download: bool=false,
      ~onClick=?,
      ~href=?,
      buttonRef,
    ) => {
    let (theme, _) = React.useContext(ThemeContext.context);
    <a
      ?href
      ?onClick
      download=?{download ? Some("") : None}
      ?disabled
      className={Controls.Styles.cta(theme.colors, ~size?, ())}
      ref=?{
        buttonRef
        |> Js.Nullable.toOption
        |> Belt.Option.map(_, ReactDOMRe.Ref.domRef)
      }>
      children
    </a>;
  });