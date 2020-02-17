[@react.component]
let make =
  React.forwardRef(
    (
      ~children,
      ~className=?,
      ~size=?,
      ~disabled=?,
      ~onClick=?,
      (),
      forwardedRef,
    ) => {
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
        forwardedRef
        |> Js.Nullable.toOption
        |> Belt.Option.map(_, ReactDOMRe.Ref.domRef)
      }>
      children
    </button>;
  });