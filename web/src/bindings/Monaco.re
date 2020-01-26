module Types = {
  type monaco;
  type disposable;

  [@bs.deriving abstract]
  type range = {
    // Line number on which the range starts (starts at 1).
    startLineNumber: int,
    // Column on which the range starts in line `startLineNumber` (starts at 1).
    startColumn: int,
    // Line number on which the range ends.
    endLineNumber: int,
    // Column on which the range ends in line `endLineNumber`.
    endColumn: int,
  };

  [@bs.deriving abstract]
  type modelContentChange = {
    // The range that got replaced.
    range,
    // The offset of the range that got replaced.
    rangeOffset: int,
    // The length of the range that got replaced.
    rangeLength: int,
    // The new text for the range.
    text: string,
  };

  [@bs.deriving abstract]
  type modelContentChangedEvent = {
    changes: array(modelContentChange),
    // The (new) end-of-line character.
    eol: string,
    // Flag that indicates that all decorations were lost with this edit.
    // The model has been reset to a new value.
    isFlush: bool,
    // Flag that indicates that this event was generated while redoing.
    isRedoing: bool,
    // Flag that indicates that this event was generated while undoing.
    isUndoing: bool,
    // The new version id the model has transitioned to.
    versionId: int,
  };

  [@bs.deriving abstract]
  type mMinimap = {enabled: bool};

  [@bs.deriving abstract]
  type options = {
    value: string,
    language: string,
    automaticLayout: bool,
    minimap: mMinimap,
  };

  [@bs.deriving abstract]
  type getValueOptions = {
    preserveBOM: bool,
    lineEnding: string,
  };
};

type t = Types.monaco;

module Theme = {
  let stringOfColors = (color: Themes.t) =>
    switch (color) {
    | Standard => "vs"
    | Dark => "vs-dark"
    | Black => "hc-black"
    };
};

external toDisposable: Types.monaco => Types.disposable = "%identity";

[@bs.send] external dispose: Types.disposable => unit = "dispose";

[@bs.module "monaco-editor"] [@bs.scope "editor"]
external create: (Dom.element, Types.options) => Types.monaco = "create";
// ⚠️ needed in order to make dyanmic imports work
let create = create;

[@bs.module "monaco-editor"] [@bs.scope "editor"]
external setTheme: string => unit = "setTheme";
let setTheme = (. theme) => Theme.stringOfColors(theme)->setTheme;

[@bs.send]
external getValue:
  (Types.monaco, Js.Nullable.t(Types.getValueOptions)) => string =
  "getValue";
let getValue =
    (~monaco: Types.monaco, ~options: option(Types.getValueOptions)=None, ()) =>
  Js.Nullable.fromOption(options) |> getValue(monaco);

[@bs.send] external layout: (Types.monaco, unit) => unit = "layout";

/**
 * calls the callback upon monaco editor
 * content change.
 * The callback parameter consists only in the actual content change -
 * to get the full content you should use *getValue* instead.
 */
[@bs.send]
external onDidChangeModelContent:
  (Types.monaco, Types.modelContentChangedEvent => unit) => Types.disposable =
  "onDidChangeModelContent";

/**
 * builds a wonka source that pushes values
 * each time the content of the monaco
 * instance changes. The pushed values
 * contain the latest updated monaco text
 * content.
 */
let makeMonacoTexthangeWonkaSource = (monaco): Wonka.Types.sourceT(string) =>
  Wonka.make((. observer: Wonka.Types.observerT(string)) => {
    let disposable =
      monaco
      |> onDidChangeModelContent(
           _,
           _ => {
             let value = getValue(~monaco, ());
             observer.next(value);
           },
         );
    (.) => dispose(disposable);
  });