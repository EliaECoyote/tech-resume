type monaco;
type t = monaco;

[@bs.deriving abstract]
type monacoMinimap = {enabled: bool};

[@bs.deriving abstract]
type monacoOptions = {
  value: string,
  language: string,
  automaticLayout: bool,
  minimap: monacoMinimap,
};

module Theme = {
  let stringOfColors = (color: Themes.t) =>
    switch (color) {
    | Standard => "vs"
    | Dark => "vs-dark"
    | Black => "hc-black"
    };
};

[@bs.module "monaco-editor"] [@bs.scope "editor"]
external create: (Dom.element, monacoOptions) => monaco = "create";

[@bs.module "monaco-editor"] [@bs.scope "editor"]
external setTheme: string => unit = "setTheme";
let setTheme = theme => Theme.stringOfColors(theme)->setTheme;

[@bs.send] external dispose: monaco => unit = "dispose";