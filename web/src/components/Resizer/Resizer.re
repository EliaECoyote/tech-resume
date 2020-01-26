[@react.component]
let make = (~children, ~className=?) => {
  <Resizer_context>
    <Resizer_content ?className> children </Resizer_content>
  </Resizer_context>;
};

module Container = Resizer_container;
module Bar = Resizer_bar;

type side = Resizer_container.side;