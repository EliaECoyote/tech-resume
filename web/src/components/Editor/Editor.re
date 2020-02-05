module Styles = {
  open Css;
  let editor = style([height(`percent(100.0))]);
};

[@react.component]
let make =
  React.forwardRef(editorRef =>
    <div
      className=Styles.editor
      ref=?{
        editorRef
        |> Js.Nullable.toOption
        |> Belt.Option.map(_, ReactDOMRe.Ref.domRef)
      }
    />
  );