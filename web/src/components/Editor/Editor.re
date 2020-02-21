module Styles = {
  open Css;
  let editor = style([height(`percent(100.0))]);
  let skeleton = style([display(`flex)]);
};

type editorDataT =
  | Content(Service.ResumeData.resumeDataT)
  | Error
  | Loading;

module type MonacoType = {include (module type of Monaco);};

[@react.component]
let make = (~editorData) => {
  let editorRef = React.useRef(Js.Nullable.null);
  let editorService = React.useContext(EditorContext.context);

  React.useEffect1(
    () =>
      Webapi.Dom.window
      |> DomHelpers.windowElement
      |> XWonka.fromDomEvent(_, "resize")
      |> XWonka.subscribe(_ => editorService.layout())
      |> XWonka.getEffectCleanup,
    [|editorService.layout|],
  );

  React.useEffect2(
    () => {
      let editorElement =
        React.Ref.current(editorRef) |> Js.Nullable.toOption;
      switch (editorData, editorElement) {
      | (Content(content), Some(editorElement)) =>
        let destroy =
          editorService.loadMonaco(editorElement, content.template);
        Some(destroy);
      | _ => None
      };
    },
    (editorData, editorService),
  );

  switch (editorData) {
  | Loading => <SkeletonPulse className=Styles.skeleton />
  | Content(_resumeData) =>
    <div className=Styles.editor ref={ReactDOMRe.Ref.domRef(editorRef)} />
  | Error => <div> <h1> {React.string("something went wrong")} </h1> </div>
  };
};