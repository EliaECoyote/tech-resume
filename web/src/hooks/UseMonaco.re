module type MonacoType = {include (module type of Monaco);};

let minimap = Monaco.Types.mMinimap(~enabled=false);
let monacoOptions =
  Monaco.Types.options(
    ~value="",
    ~language="markdown",
    ~automaticLayout=true,
    ~minimap,
  );

/**
 * react hook that manages the creation & cleanup of
 * the *Monaco editor* instance
 */
let hook = () => {
  let editorRef = React.useRef(Js.Nullable.null);
  let monacoInstanceRef: React.Ref.t(option(Monaco.t)) = React.useRef(None);

  let (state, _) = React.useContext(ThemeContext.context);

  // reacts to *theme* change by updating Monaco editor theme
  React.useEffect1(
    () => {
      let subscription =
        DynamicImport.import("../bindings/Monaco.js")
        |> DynamicImport.resolve
        |> Wonka.fromPromise
        |> Wonka.subscribe((. module AnonymousModule: MonacoType) =>
             AnonymousModule.setTheme(state.theme)
           );

      // DynamicImport.(
      //   import("../bindings/Monaco.js")
      //   |> resolve
      //   |> Js.Promise.then_((module AnonymousModule: MonacoType) => {
      //        AnonymousModule.setTheme(state.theme);
      //        Js.Promise.resolve();
      //      })
      // );
      Some(subscription.unsubscribe);
    },
    [|state.theme|],
  );

  React.useEffect0(() => {
    let subscription =
      DynamicImport.import("../bindings/Monaco.js")
      |> DynamicImport.resolve
      |> Wonka.fromPromise
      |> Wonka.subscribe((. module AnonymousModule: MonacoType) => {
           // create monaco instance
           let instance =
             React.Ref.current(editorRef)
             ->Js.Nullable.toOption
             ->Belt.Option.map(value =>
                 AnonymousModule.create(value, Obj.magic(monacoOptions))
               );

           React.Ref.setCurrent(monacoInstanceRef, Obj.magic(instance));
         });
    Some(subscription.unsubscribe);
  });

  let getContent =
    React.useCallback0(() =>
      React.Ref.current(monacoInstanceRef)
      ->Belt.Option.map(monaco => "Monaco.getValue(~monaco, ~options=None)")
    );

  (editorRef, getContent);
};