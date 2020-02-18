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
    let editorService = React.useContext(EditorContext.context);
    let (state, sendEvent) =
      UseMachine.hook(~reducer=AsyncTask.reducer, ~initialValue=Idle);
    let (resumeDataState, _saveData) =
      React.useContext(ResumeDataContext.context);

    React.useEffect1(
      () =>
        editorService.textChangeSource
        |> Wonka.subscribe((. text) =>
             React.Ref.setCurrent(editorTextRef, text)
           )
        |> WonkaHelpers.getEffectCleanup,
      [|editorService.textChangeSource|],
    );

    // md2pdf conversion api handling
    React.useEffect2(
      () =>
        switch (state) {
        | Fetching =>
          editorTextRef
          |> React.Ref.current
          |> Apis.fetchPdfData(~md=_)
          |> Wonka.subscribe((. value) =>
               switch (value) {
               | HttpClient.Ok(data) => sendEvent(LoadSuccess(data))
               | HttpClient.Failure
               | HttpClient.FailureCode(_) => sendEvent(LoadFailed)
               }
             )
          |> WonkaHelpers.getEffectCleanup
        | _ => None
        },
      (state, sendEvent),
    );

    // loads the pdf automatically, right after the
    // content has been loaded
    React.useEffect1(
      () =>
        switch (resumeDataState) {
        | Success(_data) =>
          sendEvent(LoadData);
          None;
        | _ => None
        },
      [|resumeDataState|],
    );

    let editorData =
      React.useMemo1(
        () =>
          switch (resumeDataState) {
          | Idle
          | Fetching => Editor.Loading
          | Error => Editor.Error
          | Success(data) => Editor.Content(data)
          },
        [|resumeDataState|],
      );

    <main className=Styles.app>
      <div className=Styles.header>
        <Button
          onClick={_ =>
            if (state !== Fetching) {
              sendEvent(LoadData);
            }
          }
          disabled={state == Fetching}
          className=Styles.outputTool>
          {React.string("Refresh")}
        </Button>
        <Button disabled={state == Fetching} className=Styles.outputTool>
          {React.string("Save")}
        </Button>
        <Link download="resume" href=?{makeDownloadHref(state)}>
          {React.string("Download")}
        </Link>
      </div>
      <Resizer className=Styles.resizer>
        <Resizer.Container side=Resizer_container.Left>
          <Editor editorData />
        </Resizer.Container>
        <Resizer.Bar onResizeEnd={editorService.layout} />
        <Resizer.Container side=Resizer_container.Right>
          <Output className=Styles.output requestState=state />
        </Resizer.Container>
      </Resizer>
    </main>;
  };
};

[@react.component]
let make = () => {
  <ResumeDataContext>
    <EditorContext> <PageContent /> </EditorContext>
  </ResumeDataContext>;
};

let default = make;