type state = {
  loadMonaco: React.callback(Dom.element, (string, unit) => unit),
  disposeOfMonaco: React.callback(unit, unit),
  layout: React.callback(unit, unit),
  textChangeSource: Wonka_types.sourceT(string),
};

let context =
  React.createContext({
    loadMonaco: (_, _, _) => (),
    disposeOfMonaco: () => (),
    layout: () => (),
    textChangeSource: Wonka.never,
  });

module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
  let make = React.Context.provider(context);
};

module type MonacoType = {include (module type of Monaco);};

let dynamicImportMonaco: unit => Js.Promise.t(module MonacoType) = [%bs.raw
  {| () => import("../../bindings/Monaco.bs.js") |}
];

/**
 * builds a wonka source that pushes values
 * each time the content of the monaco
 * instance changes. The pushed values
 * contain the latest updated monaco text
 * content.
 */
let makeMonacoTextSource = (monaco): Wonka.Types.sourceT(string) =>
  Wonka.make((. observer: Wonka.Types.observerT(string)) => {
    let disposable =
      monaco
      |> Monaco.onDidChangeModelContent(
           _,
           _ => {
             let value = Monaco.getValue(~monaco, ());
             observer.next(value);
           },
         );
    (.) => Monaco.dispose(disposable);
  });

[@react.component]
let make = (~children) => {
  let (textChangeSubject, _setTextChangeSubject) =
    React.useState(() => Wonka.makeSubject());
  let (themeState, _) = React.useContext(ThemeContext.context);
  let (monacoInstance, setMonacoInstance) = React.useState(() => None);

  /**
   * loads the monaco editor, given an HTMLElement.
   * returns an *unsubscribe* fn
   */
  let loadMonaco =
    React.useCallback1(
      (editorElement: Dom.element, content: string) => {
        let subscription =
          dynamicImportMonaco()
          |> Wonka.fromPromise
          |> Wonka.take(1)
          |> Wonka.subscribe((. module Monaco: MonacoType) => {
               let minimap = Monaco.Types.mMinimap(~enabled=false);
               let instance =
                 Monaco.Types.options(
                   ~value="",
                   ~language="markdown",
                   ~automaticLayout=false,
                   ~minimap,
                 )
                 |> Monaco.create(editorElement)
                 |> Obj.magic;
               setMonacoInstance(instance);
               Monaco.setValue(instance, content);
               textChangeSubject.next(content);
             });
        () => subscription.unsubscribe();
      },
      [|textChangeSubject|],
    );

  /**
   * destroys current monaco instance
   */
  let disposeOfMonaco =
    React.useCallback1(
      () => {
        switch (monacoInstance) {
        | Some(instance) =>
          Obj.magic(instance)
          // destroy monaco instance on cleanup
          |> Monaco.toDisposable
          |> Monaco.dispose;
          setMonacoInstance(_ => None);
        | None => ()
        }
      },
      [|monacoInstance|],
    );

  /**
   * layouts monaco editor.
   */
  let layout =
    React.useCallback1(
      () =>
        switch (monacoInstance) {
        | Some(instance) => Monaco.layout(instance, ())
        | None => ()
        },
      [|monacoInstance|],
    );

  // listens for monaco text changes and triggers
  React.useEffect2(
    () => {
      switch (monacoInstance) {
      | Some(monacoInstance) =>
        let disposable =
          Monaco.onDidChangeModelContent(monacoInstance, _ =>
            Monaco.getValue(~monaco=monacoInstance, ())
            |> textChangeSubject.next
          );
        Some(() => Monaco.dispose(disposable));
      | None => None
      }
    },
    (textChangeSubject, monacoInstance),
  );

  // reacts to *theme* change by updating Monaco editor theme
  React.useEffect1(
    () =>
      dynamicImportMonaco()
      |> Wonka.fromPromise
      |> Wonka.subscribe((. module Monaco: MonacoType) =>
           Monaco.setTheme(. themeState.theme)
         )
      |> WonkaHelpers.getEffectCleanup,
    [|themeState.theme|],
  );

  let textChangeSource = textChangeSubject.source;

  <Provider value={loadMonaco, layout, disposeOfMonaco, textChangeSource}>
    children
  </Provider>;
};