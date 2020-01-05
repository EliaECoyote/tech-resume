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
  let (editorRef, editorTextSource) = UseMonaco.hook();
  let (state, sendEvent) = UseMachine.hook(~reducer, ~initialValue=Idle);
  let iframeRef = React.useRef(Js.Nullable.null);
  let editorTextRef = React.useRef("");

  React.useEffect1(
    () => {
      let subscription =
        editorTextSource
        |> Wonka.subscribe((. text) =>
             text |> React.Ref.setCurrent(editorTextRef)
           );
      Some(subscription.unsubscribe);
    },
    [|editorTextSource|],
  );

  React.useEffect2(
    () =>
      switch (state) {
      | Fetching =>
        let md = editorTextRef->React.Ref.current;
        let subscription =
          Apis.fetchHtmlConversion(~md)
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
    <div>
      <p> {React.string("source (md)")} </p>
      <Editor ref={ReactDOMRe.Ref.domRef(editorRef)} />
    </div>
    <div>
      <p> {React.string("output")} </p>
      <Button onClick=startFetching disabled={state == Fetching}>
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