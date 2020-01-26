[@react.component]
let make = (~children) => {
  <Resizer_context>
    <Resizer_content> children </Resizer_content>
  </Resizer_context>;
};

let container = Resizer_container.make;
let bar = Resizer_bar.make;