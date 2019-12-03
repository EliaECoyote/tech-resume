module Styles = {
  open Css;

  let app =
    style([
      display(`grid),
      gridTemplateColumns([`percent(50.0), `percent(50.0)]),
      gridColumnGap(`pt(10)),
    ]);

  let outputContent =
    style([
      position(`absolute),
      width(`percent(100.0)),
      height(`percent(100.0)),
    ]);
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
  let (editorRef, getEditorContent) = UseMonaco.hook();
  let (iframeRef, triggerPrint) = UseOutput.hook();
  let (state, sendEvent) = UseMachine.hook(~reducer, ~initialValue=Idle);

  let startFetching = _ =>
    if (state !== Fetching) {
      sendEvent(LoadData);
    };

  React.useEffect3(
    () =>
      switch (state) {
      | Fetching =>
        let mdContent = getEditorContent()->Belt.Option.getWithDefault("");
        let subscription =
          Apis.fetchHtmlConversion(mdContent)
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
    (getEditorContent, state, sendEvent),
  );

  let outputContent =
    switch (state) {
    | Success(value) => Some(value)
    | _ => None
    };

  <div className=Styles.app>
    <div>
      <p> {React.string("source (md)")} </p>
      <Editor ref={ReactDOMRe.Ref.domRef(editorRef)} />
    </div>
    <div>
      <p> {React.string("output")} </p>
      <Button onClick=startFetching disabled={state == Fetching}>
        {React.string("Refresh")}
      </Button>
      <Button onClick=triggerPrint> {React.string("Download")} </Button>
      <span>
        {switch (state) {
         | Success(_) => React.string("Data loaded successfully!")
         | Fetching => React.string("Fetching....")
         | Error => React.string("Data failed to load")
         | Idle => React.string("Ready for some fetching!")
         }}
      </span>
      <Output>
        <iframe
          className=Styles.outputContent
          srcDoc=?outputContent
          ref={ReactDOMRe.Ref.domRef(iframeRef)}
        />
      </Output>
    </div>
  </div>;
};

let default = make;