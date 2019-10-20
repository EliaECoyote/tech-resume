module Styles = {
  open Css;

  let dropdown = (colors: ThemeContext.colors) =>
    style([
      padding2(~v=`px(6), ~h=`px(20)),
      backgroundColor(colors.background),
      border(`px(1), `solid, colors.accent),
      borderRadius(`px(0)),
      position(`relative),
      unsafe("-webkit-appearance", "none"),
      unsafe("-moz-appearance", "none"),
      textIndent(`px(1)),
      textOverflow(`string("")),
      color(colors.primary),
      fontFamily("'Montserrat', sans-serif"),
    ]);
};

[@react.component]
let make =
    (~children: ReasonReact.reactElement, ~onChange=_ => (), ~name="", ~value) => {
  let (state, _) = React.useContext(ThemeContext.context);

  <select className={Styles.dropdown(state.colors)} onChange name value>
    children
  </select>;
};

module Item = {
  [@react.component]
  let make = (~children, ~onClick=_ => (), ~value="") =>
    <option onClick value> children </option>;
};