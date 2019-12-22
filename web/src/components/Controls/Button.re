[@react.component]
let make =
  React.forwardRef(
    (~children, ~className=?, ~size=?, ~disabled=?, ~onClick=?, buttonRef) => {
    let (theme, _) = React.useContext(ThemeContext.context);
    let className =
      className
      |> Belt.Option.map(_, className => className ++ " ")
      |> Belt.Option.getWithDefault(_, "")
      |> (
        className =>
          className ++ Controls.Styles.cta(theme.colors, ~size?, ())
      );
    <button
      ?onClick
      ?disabled
      className
      ref=?{
        buttonRef
        ->Js.Nullable.toOption
        ->Belt.Option.map(ReactDOMRe.Ref.domRef)
      }>
      children
    </button>;
  });