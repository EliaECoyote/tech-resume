module Styles = {
  open Css;

  let app = style([]);
};

[@react.component]
let make = () => {
  <div className=Styles.app> <Editor /> </div>;
};

let default = make;