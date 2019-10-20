module Styles = {
  open Css;

  let editorContainer = (colors: ThemeContext.colors) =>
    style([
      padding(`px(10)),
      border(`px(2), `solid, colors.accent),
    ]);

  let editor = style([height(`vh(80.0))]);
};

[@react.component]
let make = () => {
  let editorRef = React.useRef(Js.Nullable.null);
  let monacoInstanceRef = React.useRef(None);

  let (state, _) = React.useContext(ThemeContext.context);

  React.useEffect1(
    () => {
      Monaco.setTheme(state.theme);
      None;
    },
    [|state.theme|],
  );

  React.useEffect0(() => {
    let monacoOptions =
      Monaco.monacoOptions(
        ~value="",
        ~language="markdown",
        ~automaticLayout=true,
        ~minimap=Monaco.monacoMinimap(~enabled=false),
      );

    React.Ref.current(editorRef)
    ->Js.Nullable.toOption
    ->Belt.Option.map(value => Monaco.create(value, monacoOptions))
    |> React.Ref.setCurrent(monacoInstanceRef);

    let disposeOfMonacoInstance = () =>
      React.Ref.current(monacoInstanceRef)
      ->Belt.Option.map(value => Monaco.dispose(value))
      ->(_ => ());
    Some(disposeOfMonacoInstance);
  });

  <div className={Styles.editorContainer(state.colors)}>
    <div className=Styles.editor ref={ReactDOMRe.Ref.domRef(editorRef)} />
  </div>;
};