module Styles = {
  open Css;

  let editorContainer = (colors: ThemeContext.colors) =>
    style([padding(`px(10)), border(`px(2), `solid, colors.accent)]);

  let editor = style([height(`vh(80.0))]);
};

[@react.component]
let make = () => {
  let editorRef = React.useRef(Js.Nullable.null);
  let monacoInstanceRef = React.useRef(None);

  let (state, _) = React.useContext(ThemeContext.context);

  // reacts to *theme* change by updating Monaco editor theme
  React.useEffect1(
    () => {
      Monaco.setTheme(state.theme);
      None;
    },
    [|state.theme|],
  );

  React.useEffect0(() => {
    let options =
      Monaco.Types.options(
        ~value="",
        ~language="markdown",
        ~automaticLayout=true,
        ~minimap=Monaco.Types.mMinimap(~enabled=false),
      );

    // create monaco instance
    React.Ref.current(editorRef)
    ->Js.Nullable.toOption
    ->Belt.Option.map(value => Monaco.create(value, options))
    |> React.Ref.setCurrent(monacoInstanceRef);

    // listen for editor content changes
    let changeListener =
      React.Ref.current(monacoInstanceRef)
      ->Belt.Option.map(monaco =>
          Monaco.onDidChangeModelContent(monaco, _ =>
            Js.log(Monaco.getValue(~monaco, ~options=None))
          )
        );

    // dispose of monaco instance and change listener as effect cleanup
    let cleanup = () => {
      changeListener->Belt.Option.map(Monaco.dispose)->(_ => ());
      React.Ref.current(monacoInstanceRef)
      ->Belt.Option.map(value => Monaco.toDisposable(value)->Monaco.dispose)
      ->(_ => ());
    };
    Some(cleanup);
  });

  <div className={Styles.editorContainer(state.colors)}>
    <div className=Styles.editor ref={ReactDOMRe.Ref.domRef(editorRef)} />
  </div>;
};