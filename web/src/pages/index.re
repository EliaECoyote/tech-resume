module Styles = {
  open Css;

  let app =
    style([
      height(`percent(100.0)),
      display(`grid),
      gridTemplateColumns([`fr(1.0)]),
      gridTemplateRows([`minContent, `fr(1.0)]),
      gridTemplateAreas(`areas(["header", "content"])),
      gridColumnGap(`px(10)),
      gridRowGap(`px(10)),
      alignItems(`center),
    ]);
  let header =
    style([
      gridArea(`ident("header")),
      display(`flex),
      justifyContent(`center),
    ]);
  let resizer =
    style([
      gridArea(`ident("content")),
      height(`percent(100.0)),
      border(`px(2), `solid, Css.darkcyan),
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

let buildDownloadHref = state =>
  switch (state) {
  | AsyncTask.Success(data) =>
    let options: Js2.Blob.makeBlobOptionsT = {
      type_: Some("application/pdf"),
      endings: None,
    };
    Js2.Blob.makeFromInt8Array([|data|], ~options, ())
    |> Js2.Url.createObjectURL;
  | _ => ""
  };

module PageContent = {
  [@react.component]
  let make = () => {
    open AsyncTask;
    let editorTextRef = React.useRef("");
    let editorService = React.useContext(EditorContext.context);
    let (state, sendEvent) = UseMachine.hook(~reducer, ~initialValue=Idle);
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

    let href = buildDownloadHref(state);

    <div className=Styles.app>
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
        <Link download=true disabled={href === ""} href>
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
    </div>;
  };
};

[@react.component]
let make = () => {
  <ResumeDataContext>
    <EditorContext> <PageContent /> </EditorContext>
  </ResumeDataContext>;
};

let default = make;