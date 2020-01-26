module Styles = {
  open Css;
  let resizer = style([display(`flex)]);
};

[@react.component]
let make = (~children) => {
  <div className=Styles.resizer> children </div>;
};