module Styles = {
  open Css;
  let resizer = style([display(`flex)]);
};

type side =
  | Left
  | Right;

[@react.component]
let make = (~children, ~side) => {
  let (state, dispatch) = React.useContext(Resizer_context.context);

  let flexGrow =
    switch (side) {
    | Left => state.left
    | Right => state.right
    };
  let flexGrow = Js.Float.toString(flexGrow);
  let style = ReactDOMRe.Style.make(~flex=flexGrow ++ "0 0px", ());

  <div className=Styles.resizer style> children </div>;
};