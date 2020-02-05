module type MonacoType = {include (module type of Monaco);};

let dynamicImportMonaco: unit => Js.Promise.t(module MonacoType) = [%bs.raw
  {| () => import("../bindings/Monaco.bs.js") |}
];

/**
 * react hook that manages the creation & cleanup of
 * the *Monaco editor* instance
 */
let hook = (~onTextChange: string => unit) => {
  let editorRef = React.useRef(Js.Nullable.null);
  let monacoInstanceRef = React.useRef(None);
  // managing onTextChange through a ref in order
  // to keep alive the effects
  let onTextChangeRef = React.useRef(onTextChange);
  React.Ref.setCurrent(onTextChangeRef, onTextChange);

  let (state, _) = React.useContext(ThemeContext.context);

  let layout =
    React.useCallback0(() => {
      switch (React.Ref.current(monacoInstanceRef)) {
      | Some(value) => Monaco.layout(value, ())
      | None => ()
      }
    });

  // reacts to *theme* change by updating Monaco editor theme
  React.useEffect1(
    () =>
      dynamicImportMonaco()
      |> Wonka.fromPromise
      |> Wonka.subscribe((. module Monaco: MonacoType) =>
           Monaco.setTheme(. state.theme)
         )
      |> WonkaHelpers.getEffectCleanup,
    [|state.theme|],
  );

  React.useEffect0(() => {
    let subscription =
      dynamicImportMonaco()
      |> Wonka.fromPromise
      |> Wonka.switchMap((. module Monaco: MonacoType) => {
           let minimap = Monaco.Types.mMinimap(~enabled=false);
           let monacoOptions =
             Monaco.Types.options(
               ~value="",
               ~language="markdown",
               ~automaticLayout=false,
               ~minimap,
             );

           React.Ref.current(editorRef)
           |> Js.Nullable.toOption
           |> Belt.Option.map(_, Monaco.create(_, monacoOptions))
           |> Wonka.fromValue
           |> Wonka.switchMap((. monaco) => {
                // TODO: remove Obj.magic usage
                // using Obj.magic in order to avoid error
                // "The type constructor Monaco.Types.monaco would escape its scope"
                // (caused by dynamic imports and `module type MonacoType` syntaxes)
                Obj.magic(monaco) |> React.Ref.setCurrent(monacoInstanceRef);
                Obj.magic(monaco) |> Monaco.makeMonacoTexthangeWonkaSource;
              });
         })
      |> Wonka.subscribe((. text) =>
           React.Ref.current(onTextChangeRef, text)
         );

    Some(
      () => {
        subscription.unsubscribe();
        switch (React.Ref.current(monacoInstanceRef)) {
        | Some(monacoInstance) =>
          Obj.magic(monacoInstance)
          // destroy monaco instance on cleanup
          |> Monaco.toDisposable
          |> Monaco.dispose
        | None => ()
        };
      },
    );
  });

  (editorRef, layout);
};