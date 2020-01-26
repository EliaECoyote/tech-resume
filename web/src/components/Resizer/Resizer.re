[@react.component]
let make = (~children) => {
  <Resizer_context>
    <Resizer_content> children </Resizer_content>
  </Resizer_context>;
};

module Container = Resizer_container;
module Bar = Resizer_bar;