module Styles = {
  open Css;
  let resizer =
    style([overflowX(`hidden), transition(~duration=200, "filter")]);
  let resizing = style([filter([`blur(`px(5))])]);
};

type side =
  | Left
  | Right;

[@react.component]
let make = (~children, ~side) => {
  let (state, _dispatch) = React.useContext(Resizer_context.context);

  let flexGrow =
    switch (side) {
    | Left => state.left
    | Right => state.right
    };

  let flexGrow = Js.Float.toString(flexGrow);
  let style = ReactDOMRe.Style.make(~flex=flexGrow ++ " 0 0px", ());

  let className =
    state.resizing ? Styles.resizer ++ " " ++ Styles.resizing : Styles.resizer;

  <div className style> children </div>;
};