[@bs.deriving abstract]
type monacoOptions = {
  value: string,
  language: string,
};

[@bs.module "monaco-editor"] [@bs.scope "editor"]
external startupMonaco: (Dom.element, monacoOptions) => unit = "create"
