module Styles = {
  open Css;

  let disabledRules = [opacity(0.3)];

  let link =
      (colors: ThemeContext.colors, ~size=Controls.Large, ~empty=false, ()) =>
    merge([
      Controls.Styles.cta(colors, ~size, ()),
      style(empty ? Controls.Styles.disabledRules : []),
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
    let empty = Belt.Option.isNone(href);
    <a
      ?href
      ?onClick
      download=?{download ? Some("") : None}
      className={Styles.link(theme.colors, ~size?, ~empty, ())}
      ref=?{
        buttonRef
        |> Js.Nullable.toOption
        |> Belt.Option.map(_, ReactDOMRe.Ref.domRef)
      }>
      children
    </a>;
  });