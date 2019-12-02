module Styles = {
  open Css;

  let app =
    style([
      display(`grid),
      gridTemplateColumns([`percent(50.0), `percent(50.0)]),
      gridColumnGap(`pt(10)),
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

  let startFetching = _ => sendEvent(LoadData);

  React.useEffect3(
    () =>
      switch (state) {
      | Fetching =>
        let mdContent = getEditorContent()->Belt.Option.getWithDefault("");
        let url =
          Url.make(
            ~scheme="http",
            ~host="127.0.0.1:3000",
            ~path="convert",
            ~qsComponents=[|("md", mdContent)|],
            (),
          );
        let subscription =
          HttpClient.get(~resource=url)
          |> Wonka.concatMap((. value) => HttpClient.toText(value))
          |> Wonka.subscribe((. value) => Js.log(value));
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
      <Button onClick=startFetching> {React.string("Refresh")} </Button>
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
          srcDoc=?outputContent
          ref={ReactDOMRe.Ref.domRef(iframeRef)}
        />
      </Output>
    </div>
  </div>;
};

let default = make;