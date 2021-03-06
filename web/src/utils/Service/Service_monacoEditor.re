module type MonacoType = {include (module type of Monaco);};

type monacoEditorServiceT = {
  textChangeSource: Wonka_types.sourceT(string),
  loadMonaco: (Dom.element, string, unit) => unit,
  setTheme: Themes.t => unit,
  layout: unit => unit,
};

let make = (): monacoEditorServiceT => {
  let monaco = ref(None);
  let textChangeSubject = XWonka.makeReplaySubject(1);
  let dynamicImportMonaco: unit => Js.Promise.t(module MonacoType) = [%bs.raw
    {| () => import(
      /* webpackChunkName: "monaco" */
      "../../bindings/Monaco.bs.js"
    ) |}
  ];
  let dynamicImportMonaco = () =>
    dynamicImportMonaco() |> XWonka.fromPromise |> XWonka.take(1);

  /**
   * destroys current monaco instance
   */
  let disposeOfMonaco = () => {
    let _ =
      dynamicImportMonaco()
      |> XWonka.subscribe((module Monaco: MonacoType) =>
           switch (monaco^) {
           | Some(instance) =>
             Obj.magic(instance)
             // destroy monaco instance on cleanup
             |> Monaco.toDisposable
             |> Monaco.dispose;
             monaco := None;
           | None => ()
           }
         );
    ();
  };

  /**
   * loads the monaco editor, given an HTMLElement.
   * returns a *destroy* fn
   */
  let loadMonaco = (editorElement: Dom.element, content: string) => {
    let subscription =
      dynamicImportMonaco()
      // initializes monaco instance
      |> XWonka.onPush((module Monaco: MonacoType) =>
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
      |> XWonka.mergeMap((module Monaco: MonacoType) =>
           switch (monaco^) {
           | Some(monaco) =>
             XWonka.make(observer => {
               observer.next(content);
               let monaco = Obj.magic(monaco);
               let disposable =
                 Monaco.onDidChangeModelContent(monaco, _event =>
                   observer.next @@ Monaco.getValue(~monaco, ())
                 );
               (() => Monaco.dispose(disposable));
             })
             // *shareReplay* makes sure that the first *observer.next*
             // invocation is received by the subscriber
             |> XWonka.shareReplay(1)
           | None => XWonka.never
           }
         )
      |> XWonka.onPush(textChangeSubject.next)
      |> XWonka.publish;
    () => {
      subscription.unsubscribe();
      disposeOfMonaco();
    };
  };

  /**
   * layouts monaco editor.
   */
  let layout = () => {
    let _ =
      dynamicImportMonaco()
      |> XWonka.subscribe((module Monaco: MonacoType) =>
           switch (monaco^) {
           | Some(instance) => Monaco.layout(Obj.magic(instance), ())
           | None => ()
           }
         );
    ();
  };

  /**
   * changes editor theme.
   */
  let setTheme = theme => {
    let _ =
      dynamicImportMonaco()
      |> XWonka.subscribe((module Monaco: MonacoType) =>
           Monaco.setTheme @@ Monaco.Theme.stringOfColors(theme)
         );
    ();
  };

  // expose only the source of the subject from
  // the service
  let textChangeSource = textChangeSubject.source;

  {textChangeSource, loadMonaco, setTheme, layout};
};