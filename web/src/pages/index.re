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
    style([gridArea(`ident("content")), height(`percent(100.0))]);
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

[@react.component]
let make = () => {
  open AsyncTask;
  let (editorRef, layout, editorTextSource) = UseMonaco.hook();
  let (state, sendEvent) = UseMachine.hook(~reducer, ~initialValue=Idle);
  let editorTextRef = React.useRef("");

  // *editorRef* value updates handling
  React.useEffect1(
    () =>
      editorTextSource
      |> Wonka.subscribe((. text) =>
           React.Ref.setCurrent(editorTextRef, text)
         )
      |> WonkaHelpers.getEffectCleanup,
    [|editorTextSource|],
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

  let startFetching = _ =>
    if (state !== Fetching) {
      sendEvent(LoadData);
    };

  <div className=Styles.app>
    <div className=Styles.header>
      <Button
        onClick=startFetching
        disabled={state == Fetching}
        className=Styles.outputTool>
        {React.string("Refresh")}
      </Button>
      // <Link
      //   download=true
      //   href={Url.make(
      //     ~scheme=Config.pdfgenScheme,
      //     ~host=Config.pdfgenHost,
      //     ~path="",
      //     ~qsComponents=[|("html", html)|],
      //     (),
      //   )}>
      //   {React.string("Download")}
      // </Link>
      <span>
        {switch (state) {
         | Success(_) => React.string("Data loaded successfully!")
         | Fetching => React.string("Fetching....")
         | Error => React.string("Data failed to load")
         | Idle => React.string("Ready for some fetching!")
         }}
      </span>
    </div>
    <Resizer className=Styles.resizer>
      <Resizer.Container side=Resizer_container.Left>
        <Editor ref={ReactDOMRe.Ref.domRef(editorRef)} />
      </Resizer.Container>
      <Resizer.Bar onResizeEnd=layout />
      <Resizer.Container side=Resizer_container.Right>
        <Output className=Styles.output requestState=state />
      </Resizer.Container>
    </Resizer>
  </div>;
};

let default = make;