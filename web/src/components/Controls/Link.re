[@react.component]
let make =
  React.forwardRef(
    (
      ~children,
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
      className={Controls.Styles.cta(theme.colors)}
      ref=?{
        buttonRef
        ->Js.Nullable.toOption
        ->Belt.Option.map(ReactDOMRe.Ref.domRef)
      }>
      children
    </a>;
  });