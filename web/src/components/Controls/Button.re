[@react.component]
let make =
  React.forwardRef((~children, ~disabled=?, ~onClick=?, buttonRef) => {
    let (theme, _) = React.useContext(ThemeContext.context);
    <button
      ?onClick
      ?disabled
      className={Controls.Styles.cta(theme.colors)}
      ref=?{
        buttonRef
        ->Js.Nullable.toOption
        ->Belt.Option.map(ReactDOMRe.Ref.domRef)
      }>
      children
    </button>;
  });