module Styles = {
  open Css;

  let app = style([height(px(200)), backgroundColor(blue)]);
};

[@react.component]
let make = () => {
  <div> <ThemeDropdown /> <Editor /> </div>;
};

let default = make;