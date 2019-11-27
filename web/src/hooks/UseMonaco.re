/**
 * react hook that manages the creation & cleanup of
 * the *Monaco editor* instance
 */
let hook = () => {
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
    let minimap = Monaco.Types.mMinimap(~enabled=false);
    let options =
      Monaco.Types.options(
        ~value="",
        ~language="markdown",
        ~automaticLayout=true,
        ~minimap,
      );

    // create monaco instance
    let instance =
      React.Ref.current(editorRef)
      ->Js.Nullable.toOption
      ->Belt.Option.map(value => Monaco.create(value, options));

    React.Ref.setCurrent(monacoInstanceRef, instance);

    // dispose of monaco instance as effect cleanup
    let cleanup = () =>
      switch (instance) {
      | Some(monaco) => monaco |> Monaco.toDisposable |> Monaco.dispose
      | None => ()
      };
    Some(cleanup);
  });

  let getContent =
    React.useCallback0(() =>
      React.Ref.current(monacoInstanceRef)
      ->Belt.Option.map(monaco => Monaco.getValue(~monaco, ~options=None))
    );

  (editorRef, getContent);
};