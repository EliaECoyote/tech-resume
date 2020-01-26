[@react.component]
let make =
  React.forwardRef(
    (~children, ~className=?, ~size=?, ~disabled=?, ~onClick=?, buttonRef) => {
    let (theme, _) = React.useContext(ThemeContext.context);
    let className =
      CssHelpers.combine([
        Some(Controls.Styles.cta(theme.colors, ~size?, ())),
        className,
      ]);
    <button
      ?onClick
      ?disabled
      className
      ref=?{
        buttonRef
        |> Js.Nullable.toOption
        |> Belt.Option.map(_, ReactDOMRe.Ref.domRef)
      }>
      children
    </button>;
  });