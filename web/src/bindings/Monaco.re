type monaco;
type t = monaco;

[@bs.deriving abstract]
type monacoOptions = {
  value: string,
  language: string,
};

module Theme = {
  type t =
    | Standard
    | Dark
    | Black;
  let stringOfColors = color =>
    switch (color) {
    | Standard => "vs"
    | Dark => "vs-dar"
    | Black => "hc-black"
    };
};

[@bs.module "monaco-editor"] [@bs.scope "editor"]
external startupMonaco: (Dom.element, monacoOptions) => monaco = "create";

[@bs.send] external setTheme: (monaco, string) => unit = "setTheme";
let setTheme = (monaco, color) =>
  Theme.stringOfColors(color) |> setTheme(monaco);