module Styles = {
  open Css;

  let disabledRules = [opacity(0.3)];

  let link =
      (colors: ThemeContext.colors, ~size=Controls.Large, ~disabled=false, ()) =>
    merge([
      Controls.Styles.cta(colors, ~size, ~disabled, ()),
      style(disabled ? Controls.Styles.disabledRules : []),
    ]);
};

[@react.component]
let make =
  React.forwardRef(
    (
      ~children,
      ~size=?,
      ~download: bool=false,
      ~onClick=?,
      ~href=?,
      buttonRef,
    ) => {
    let (theme, _) = React.useContext(ThemeContext.context);
    let disabled = Belt.Option.isNone(href);
    <a
      ?href
      ?onClick
      disabled
      download=?{download ? Some("") : None}
      className={Styles.link(theme.colors, ~size?, ~disabled, ())}
      ref=?{
        buttonRef
        |> Js.Nullable.toOption
        |> Belt.Option.map(_, ReactDOMRe.Ref.domRef)
      }>
      children
    </a>;
  });