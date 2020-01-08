module Styles = {
  open Css;

  let app =
    style([
      height(`percent(100.0)),
      display(`grid),
      gridTemplateColumns([`minContent, `fr(1.0)]),
      gridTemplateRows([`minContent, `fr(1.0)]),
      gridTemplateAreas(
        `areas(["editor-header output-header", "editor output"]),
      ),
      gridColumnGap(`px(10)),
      gridRowGap(`px(10)),
      alignItems(`center),
    ]);
  let editorHeader = style([gridArea(`ident("editor-header"))]);
  let outputHeader = style([gridArea(`ident("output-header"))]);
  let editor = (colors: ThemeContext.colors) =>
    style([
      gridArea(`ident("editor")),
      resize(`horizontal),
      overflow(`auto),
      minWidth(`vw(30.0)),
      width(`vw(45.0)),
      border(`px(2), `solid, colors.accent),
      height(`percent(100.0)),
      display(`flex),
      flexDirection(`column),
    ]);
  let output =
    style([
      gridArea(`ident("output")),
      minWidth(`vw(30.0)),
      height(`percent(100.0)),
    ]);
  let outputTool =
    style([
      margin4(~top=`zero, ~right=`px(10), ~bottom=`zero, ~left=`zero),
    ]);
  let outputContent =
    style([width(`percent(100.0)), height(`percent(100.0))]);
};

type status =
  | Idle
  | Fetching
  | Error
  | Success(string);

type events =
  | LoadData
  | LoadSuccess(string)
  | LoadFailed;

let reducer = (~state, ~event) =>
  switch (state, event) {
  | (_, LoadData) => Fetching->UseMachine.Valid
  | (Fetching, LoadSuccess(value)) => Success(value)->UseMachine.Valid
  | (Fetching, LoadFailed) => Error->UseMachine.Valid
  | _ => UseMachine.Invalid(("invalid template machine event", state, event))
  };

[@react.component]
let make = () => {
  let (editorRef, editorTextSource) = UseMonaco.hook();
  let (state, sendEvent) = UseMachine.hook(~reducer, ~initialValue=Idle);
  let (themeState, _) = React.useContext(ThemeContext.context);
  let iframeRef = React.useRef(Js.Nullable.null);
  let editorTextRef = React.useRef("");

  // *editorRef* value updates handling
  React.useEffect1(
    () => {
      let subscription =
        editorTextSource
        |> Wonka.subscribe((. text) =>
             React.Ref.setCurrent(editorTextRef, text)
           );
      Some(subscription.unsubscribe);
    },
    [|editorTextSource|],
  );

  // html conversion api handling
  React.useEffect2(
    () =>
      switch (state) {
      | Fetching =>
        let subscription =
          editorTextRef
          |> React.Ref.current
          |> Apis.fetchHtmlConversion(~md=_)
          |> Wonka.subscribe((. value) =>
               switch (value) {
               | HttpClient.Ok(html) => sendEvent(LoadSuccess(html))
               | HttpClient.Failure
               | HttpClient.FailureCode(_) => sendEvent(LoadFailed)
               }
             );
        Some(subscription.unsubscribe);
      | _ => None
      },
    (state, sendEvent),
  );

  let startFetching = _ =>
    if (state !== Fetching) {
      sendEvent(LoadData);
    };

  let outputContent =
    switch (state) {
    | Success(value) => Some(value)
    | _ => None
    };

  let html = Belt.Option.getWithDefault(outputContent, "");

  <div className=Styles.app>
    <div className=Styles.editorHeader>
      <span> {React.string("source (md)")} </span>
    </div>
    <div className={Styles.editor(themeState.colors)}>
      <Editor ref={ReactDOMRe.Ref.domRef(editorRef)} />
      <ResizerIndicator />
    </div>
    <div className=Styles.outputHeader>
      <Button
        onClick=startFetching
        disabled={state == Fetching}
        className=Styles.outputTool>
        {React.string("Refresh")}
      </Button>
      <Link
        download=true
        href={Url.make(
          ~scheme=Config.pdfgenScheme,
          ~host=Config.pdfgenHost,
          ~path="",
          ~qsComponents=[|("html", html)|],
          (),
        )}>
        {React.string("Download")}
      </Link>
      <span>
        {switch (state) {
         | Success(_) => React.string("Data loaded successfully!")
         | Fetching => React.string("Fetching....")
         | Error => React.string("Data failed to load")
         | Idle => React.string("Ready for some fetching!")
         }}
      </span>
    </div>
    <div className=Styles.output>
      <iframe
        className=Styles.outputContent
        srcDoc=?outputContent
        ref={ReactDOMRe.Ref.domRef(iframeRef)}
      />
    </div>
  </div>;
};

let default = make;