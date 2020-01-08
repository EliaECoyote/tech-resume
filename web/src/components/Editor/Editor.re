module Styles = {
  open Css;
  let container = style([flex(`num(1.0))]);
  let editor = style([height(`percent(100.0))]);
};

[@react.component]
let make =
  React.forwardRef(editorRef => {
    let (state, _) = React.useContext(ThemeContext.context);
    <div className=Styles.container>
      <div
        className=Styles.editor
        ref=?{
          editorRef
          ->Js.Nullable.toOption
          ->Belt.Option.map(ReactDOMRe.Ref.domRef)
        }
      />
    </div>;
  });