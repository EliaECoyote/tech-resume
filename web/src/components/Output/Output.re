module Styles = {
  open Css;

  let output =
    style([
      display(`grid),
      gridTemplateColumns([`percent(50.0), `percent(50.0)]),
      gridColumnGap(`pt(10)),
    ]);
};

[@react.component]
let make = (~children) => {
  <div className=Styles.output> children </div>;
};

let default = make;