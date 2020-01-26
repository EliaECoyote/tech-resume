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