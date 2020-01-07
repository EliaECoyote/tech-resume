module Styles = {
  open Css;

  let editorContainer = (colors: ThemeContext.colors) =>
    style([
      height(`percent(100.0)),
      border(`px(2), `solid, colors.accent),
    ]);

  let editor = style([height(`percent(100.0))]);
};

[@react.component]
let make =
  React.forwardRef(editorRef => {
    let (state, _) = React.useContext(ThemeContext.context);
    <div className={Styles.editorContainer(state.colors)}>
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