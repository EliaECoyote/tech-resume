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
  let (editorRef, _) = Hooks.useMonaco();

  <div className=Styles.app>
    <div>
      <p> {React.string("source (md)")} </p>
      <Editor ref={ReactDOMRe.Ref.domRef(editorRef)} />
    </div>
    <div> <p> {React.string("output")} </p> <Output /> </div>
  </div>;
};

let default = make;