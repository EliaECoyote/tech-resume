module Styles = {
  open Css;

  let app = style([height(px(200)), backgroundColor(blue)]);
};

[@react.component]
let make = () => {
  <ConfigContext> <div> <ThemeDropdown /> <Editor /> </div> </ConfigContext>;
};

let default = make;