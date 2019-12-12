module type MonacoType = {include (module type of Monaco);};

let dynamicImportMonaco: unit => Js.Promise.t(module MonacoType) = [%bs.raw
  {| () => import("../bindings/Monaco.js") |}
];

/**
 * react hook that manages the creation & cleanup of
 * the *Monaco editor* instance
 */
let hook = () => {
  let editorRef = React.useRef(Js.Nullable.null);
  let (textSubject, _) = React.useState(() => Wonka.makeSubject());
  let monacoInstanceRef = React.useRef(None);

  let (state, _) = React.useContext(ThemeContext.context);

  let {Wonka.Types.source: textSource, Wonka.Types.next: textNext} = textSubject;

  // reacts to *theme* change by updating Monaco editor theme
  React.useEffect1(
    () => {
      let subscription =
        dynamicImportMonaco()
        |> Wonka.fromPromise
        |> Wonka.subscribe((. module Monaco: MonacoType) =>
             Monaco.setTheme(. state.theme)
           );
      Some(subscription.unsubscribe);
    },
    [|state.theme|],
  );

  React.useEffect1(
    () => {
      // monaco library dynamic import source
      let importSource =
        dynamicImportMonaco() |> Wonka.fromPromise |> Wonka.share;

      // monaco instance wonka source
      let monacoInstanceSource =
        importSource
        |> Wonka.map((. module Monaco: MonacoType) => {
             let minimap = Monaco.Types.mMinimap(~enabled=false);
             let monacoOptions =
               Monaco.Types.options(
                 ~value="",
                 ~language="markdown",
                 ~automaticLayout=true,
                 ~minimap,
               );
             // TODO: remove Obj.magic usage
             // using Obj.magic in order to avoid error
             // "The type constructor Monaco.Types.monaco would escape its scope"
             // (caused by dynamic imports and `module type MonacoType` syntaxes)
             React.Ref.current(editorRef)
             ->Js.Nullable.toOption
             ->Belt.Option.map(value => Monaco.create(value, monacoOptions))
             ->Obj.magic;
           })
        |> Wonka.share;

      // sets the current monaco instance in a react ref
      let monacoInstanceRefSubscription =
        monacoInstanceSource
        |> Wonka.subscribe((. monaco) =>
             React.Ref.setCurrent(monacoInstanceRef, monaco)
           );

      // pushes events to *textSource* when monaco text changes
      let monacoTextChangeSubscription =
        Wonka.combine(importSource, monacoInstanceSource)
        |> Wonka.take(1)
        |> Wonka.mergeMap((. (module Monaco: MonacoType, monaco))
             // TODO: remove Obj.magic usage
             // using Obj.magic in order to avoid error
             // "The type constructor Monaco.Types.monaco would escape its scope"
             // (caused by dynamic imports and `module type MonacoType` syntaxes)
             => monaco |> Obj.magic |> Monaco.makeMonacoTexthangeWonkaSource)
        |> Wonka.subscribe((. text) => textNext(text));

      Some(
        () => {
          monacoInstanceRefSubscription.unsubscribe();
          monacoTextChangeSubscription.unsubscribe();
        },
      );
    },
    [|textNext|],
  );

  (editorRef, textSource);
};