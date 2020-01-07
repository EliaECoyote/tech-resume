module Styles = {
  open Css;

  let output =
    style([
      display(`grid),
      gridTemplateColumns([`percent(50.0), `percent(50.0)]),
      gridColumnGap(`pt(10)),
      backgroundColor(`hex("fff")),
      height(`px(0)),
      paddingTop(`percent(141.4)),
      position(relative),
    ]);
};

[@react.component]
let make = (~children, ~className=?) => {
  let className =
    className
    |> Belt.Option.map(_, value => Css.merge([Styles.output, value]))
    |> Belt.Option.getWithDefault(_, Styles.output);
  <div className={Css.merge([Styles.output, className])}> children </div>;
};

let default = make;