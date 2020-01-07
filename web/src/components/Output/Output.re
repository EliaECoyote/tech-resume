module Styles = {
  open Css;
  let output = style([height(`percent(100.0))]);
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