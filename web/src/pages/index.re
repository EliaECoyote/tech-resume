module Styles = {
  open Css;

  let app =
    style([
      height(`percent(100.0)),
      display(`grid),
      gridTemplateColumns([`fr(1.0)]),
      gridTemplateRows([`minContent, `fr(1.0)]),
      gridTemplateAreas(`areas(["toolbar", "editor-and-output"])),
      gridColumnGap(`px(10)),
      gridRowGap(`px(10)),
      alignItems(`center),
    ]);
  let header =
    style([
      gridArea(`ident("toolbar")),
      display(`flex),
      justifyContent(`center),
    ]);
  let resizer =
    style([
      gridArea(`ident("editor-and-output")),
      height(`percent(100.0)),
      border(`px(2), `solid, Css.darkcyan),
      overflow(`hidden),
    ]);
  let editor = (colors: ThemeContext.colors) =>
    style([
      border(`px(2), `solid, colors.accent),
      height(`percent(100.0)),
      display(`flex),
      flexDirection(`column),
    ]);
  let output = style([height(`percent(100.0))]);
  let outputTool =
    style([
      margin4(~top=`zero, ~right=`px(10), ~bottom=`zero, ~left=`zero),
    ]);
};

let makeDownloadHref = state =>
  switch (state) {
  | AsyncTask.Success(data) =>
    let options: Js2.Blob.makeBlobOptionsT = {
      type_: Some("application/pdf"),
      endings: None,
    };
    Js2.Blob.makeFromInt8Array([|data|], ~options, ())
    |> Js2.Url.createObjectURL
    |> (url => Some(url));
  | _ => None
  };

module PageContent = {
  [@react.component]
  let make = () => {
    let editorTextRef = React.useRef("");
    let (isSaving, setIsSaving) = React.useState(() => false);
    let (authStatus, _) = React.useContext(AuthContext.context);
    let editorService = React.useContext(EditorContext.context);
    let resumeDataService =
      Belt2.React.useSafeContext(ResumeDataServiceContext.context);
    let resumeTask = UseResumeFetchTask.hook();
    let disposeBag = UseDisposeBag.hook();
    let (pdfDataTask, fetchPdfData) = UsePDFData.hook();

    React.useEffect1(
      () =>
        editorService.textChangeSource
        |> Wonka.subscribe((. text) =>
             React.Ref.setCurrent(editorTextRef, text)
           )
        |> WonkaHelpers.getEffectCleanup,
      [|editorService.textChangeSource|],
    );

    // loads the pdf automatically, right after the
    // content has been loaded
    React.useEffect1(
      () =>
        switch (resumeTask) {
        | Success(data) =>
          fetchPdfData(data.template);
          None;
        | _ => None
        },
      [|resumeTask|],
    );

    let editorData =
      React.useMemo1(
        () =>
          switch (resumeTask) {
          | Idle
          | Fetching => Editor.Loading
          | Error => Editor.Error
          | Success(data) => Editor.Content(data)
          },
        [|resumeTask|],
      );

    let canSaveData =
      !isSaving
      && (
        switch (authStatus, resumeTask) {
        | (UseAuth.Anonymous | UseAuth.Logged(_), AsyncTask.Success(_)) =>
          true
        | (_, _) => false
        }
      );

    <main className=Styles.app>
      <div className=Styles.header>
        <Button
          onClick={_ =>
            if (pdfDataTask !== Fetching) {
              let _ =
                editorService.textChangeSource
                |> Wonka.take(1)
                |> Wonka.subscribe((. text) => fetchPdfData(text));
              ();
            }
          }
          disabled={pdfDataTask == Fetching}
          className=Styles.outputTool>
          {React.string("Refresh")}
        </Button>
        <Button
          disabled={!canSaveData}
          className=Styles.outputTool
          onClick={_ =>
            switch (resumeTask) {
            | AsyncTask.Success(resume) =>
              setIsSaving(_ => true);
              let newResume: ServiceResumeData.resumeDataT = {
                theme: resume.theme,
                template: React.Ref.current(editorTextRef),
              };
              resumeDataService.saveResume(authStatus, newResume)
              |> Wonka.subscribe((. result) => {
                   setIsSaving(_ => false);
                   switch (result) {
                   | Belt.Result.Ok(_) =>
                     Js2.Global.alert("save operation success")
                   | Belt.Result.Error(error) =>
                     Js.Console.error(error);
                     Js2.Global.alert("save operation error");
                   };
                 })
              |> disposeBag.add;
            | _ => ()
            }
          }>
          {React.string("Save")}
        </Button>
        <Link download="resume" href=?{makeDownloadHref(pdfDataTask)}>
          {React.string("Download")}
        </Link>
      </div>
      <Resizer className=Styles.resizer>
        <Resizer.Container side=Resizer_container.Left>
          <Editor editorData />
        </Resizer.Container>
        <Resizer.Bar onResizeEnd={editorService.layout} />
        <Resizer.Container side=Resizer_container.Right>
          <Output className=Styles.output requestState=pdfDataTask />
        </Resizer.Container>
      </Resizer>
    </main>;
  };
};

[@react.component]
let make = () => {
  <ResumeDataServiceContext>
    <EditorContext> <PageContent /> </EditorContext>
  </ResumeDataServiceContext>;
};

let default = make;