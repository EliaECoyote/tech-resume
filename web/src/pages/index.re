module Styles = {
  open Css;

  let app =
    style([
      display(`grid),
      gridTemplateColumns([`percent(50.0), `percent(50.0)]),
      gridColumnGap(`pt(10)),
    ]);
};

[@react.component]
let make = () => {
  <div className=Styles.app>
    <div> <p> {React.string("source (md)")} </p> <Editor /> </div>
    <div> <p> {React.string("output")} </p> </div>
  </div>;
};

let default = make;