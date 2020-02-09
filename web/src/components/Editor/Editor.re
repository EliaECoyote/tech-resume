module Styles = {
  open Css;
  let editor = style([height(`percent(100.0))]);
  let skeleton = (colors: ThemeContext.colors) => {
    style([
      display(`flex),
      backgroundColor(colors.background),
      unsafe(
        "backgroundImage",
        "linear-gradient(90deg, #282c33, #000, #282c33)",
      ),
    ]);
  };
};

type editorDataT =
  | Content(UseResumeData.resumeDataT)
  | Error
  | Loading;

module type MonacoType = {include (module type of Monaco);};

let dynamicImportMonaco: unit => Js.Promise.t(module MonacoType) = [%bs.raw
  {| () => import("../../bindings/Monaco.bs.js") |}
];

[@react.component]
let make = (~editorData) => {
  let (theme, _) = React.useContext(ThemeContext.context);
  let editorRef = React.useRef(Js.Nullable.null);
  let editorService = React.useContext(EditorContext.context);

  React.useEffect2(
    () => {
      let editorElement =
        React.Ref.current(editorRef) |> Js.Nullable.toOption;

      switch (editorData, editorElement) {
      | (Content(content), Some(editorElement)) =>
        let unsubscribe =
          editorService.loadMonaco(editorElement, content.template);
        Some(unsubscribe);
      | _ => None
      };
    },
    (editorData, editorService.loadMonaco),
  );

  React.useEffect1(
    () => Some(() => editorService.disposeOfMonaco()),
    [|editorService.disposeOfMonaco|],
  );

  switch (editorData) {
  | Loading => <SkeletonPulse className={Styles.skeleton(theme.colors)} />
  | Content(_resumeData) =>
    <div className=Styles.editor ref={ReactDOMRe.Ref.domRef(editorRef)} />
  | Error => <div> <h1> {React.string("something went wrong")} </h1> </div>
  };
};