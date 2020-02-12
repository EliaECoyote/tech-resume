module type MonacoType = {include (module type of Monaco);};

type monacoEditorServiceT = {
  monaco: Pervasives.ref(option(Monaco.Types.monaco)),
  textChangeSource: Wonka_types.sourceT(string),
  loadMonaco: (Dom.element, string, unit) => unit,
  layout: unit => unit,
};

let dynamicImportMonaco: unit => Js.Promise.t(module MonacoType) = [%bs.raw
  {| () => import("../bindings/Monaco.bs.js") |}
];

let dynamicImportMonaco = () =>
  dynamicImportMonaco() |> Wonka.fromPromise |> Wonka.take(1);

let make = (): monacoEditorServiceT => {
  let monaco = ref(None);
  let textChangeSubject = Wonka.makeSubject();

  /**
   * destroys current monaco instance
   */
  let disposeOfMonaco = () =>
    switch (monaco^) {
    | Some(instance) =>
      Obj.magic(instance)
      // destroy monaco instance on cleanup
      |> Monaco.toDisposable
      |> Monaco.dispose;
      monaco := None;
    | None => ()
    };

  /**
   * loads the monaco editor, given an HTMLElement.
   * returns a *destroy* fn
   */
  let loadMonaco = (editorElement: Dom.element, content: string) => {
    let subscription =
      dynamicImportMonaco()
      // initializes monaco instance
      |> Wonka.onPush((. module Monaco: MonacoType) =>
           if (Belt.Option.isNone(monaco^)) {
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
             monaco := Some(instance);
             Monaco.setValue(instance, content);
           }
         )
      // maps to a text-change source
      |> Wonka.mergeMap((. module Monaco: MonacoType) =>
           switch (monaco^) {
           | Some(monaco) =>
             Wonka.make((. observer: Wonka.Types.observerT(string)) => {
               observer.next(content);
               let monaco = Obj.magic(monaco);
               let disposable =
                 Monaco.onDidChangeModelContent(monaco, _event =>
                   observer.next @@ Monaco.getValue(~monaco, ())
                 );
               ((.) => Monaco.dispose(disposable));
             })
           | None => Wonka.never
           }
         )
      |> Wonka.onPush((. value) => textChangeSubject.next(value))
      |> Wonka.publish;
    () => {
      subscription.unsubscribe();
      disposeOfMonaco();
    };
  };

  /**
   * layouts monaco editor.
   */
  let layout = () =>
    switch (monaco^) {
    | Some(instance) => Monaco.layout(instance, ())
    | None => ()
    };

  // expose only the source of the subject from
  // the service
  let textChangeSource = textChangeSubject.source;

  {textChangeSource, monaco, loadMonaco, layout};
};