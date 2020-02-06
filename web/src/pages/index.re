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

let getDefaultResumeObject = () => {
  let resume = Js.Dict.empty();
  Js.Dict.set(resume, "template", Js.Json.string(""));
  Js.Dict.set(resume, "theme", Js.Json.string("124aa2e3"));
  Js.Json.object_(resume);
};

[@react.component]
let make = () => {
  open AsyncTask;
  let editorTextRef = React.useRef("");
  let (editorRef, layout) =
    UseMonaco.hook(~onTextChange=React.Ref.setCurrent(editorTextRef));
  let (state, sendEvent) = UseMachine.hook(~reducer, ~initialValue=Idle);
  let (authStatus, _signOut) = React.useContext(AuthContext.context);

  React.useEffect1(
    () => {
      switch (authStatus) {
      | Logged(user) =>
        let db = Firebase.Firestore.make();
        let uid = user.uid;
        let collectionPath = {j|users/$uid/resumesDetails|j};
        let resumesDetailsRef =
          Firebase.Firestore.collection(db, ~collectionPath);
        Js.log(("requesting", collectionPath));
        // Firebase.Firestore.DocumentReference.setWithOptions(
        //   userDocRef,
        //   ~value=getDefaultUserObject(),
        //   ~options={merge: Some(true), mergeFields: None},
        // )
        // let data = getDefaultUserObject();
        // let documentRef =
        //   Firebase.Firestore.CollectionReference.add(
        //     resumesDetailsRef,
        //     ~data,
        //   );
        // Js.log(("document ref: ", documentRef));
        // Js.log((
        //   "document ID: ",
        //   Firebase.Firestore.DocumentReference.get_id(documentRef),
        // ));
        // |> Firebase.Firestore.DocumentReference.
        Firebase.Firestore.CollectionReference.get(resumesDetailsRef)
        |> WonkaHelpers.fromPromise
        |> Wonka.subscribe((. snapshot) => {
             let _ =
               switch (snapshot) {
               | Belt.Result.Ok(snapshot) =>
                 let empty =
                   Firebase.Firestore.QuerySnapshot.get_empty(snapshot);
                 if (empty) {
                   let _ =
                     Firebase.Firestore.CollectionReference.add(
                       resumesDetailsRef,
                       ~data=getDefaultResumeObject(),
                     )
                     |> WonkaHelpers.fromPromise
                     |> Wonka.subscribe((. value) =>
                          Js.log(("default resume value added", value))
                        );
                   ();
                 };
                 Js.log2("empty: ", empty);
               | Belt.Result.Error(error) => Js.Console.error(error)
               };
             Js.log(snapshot);
           })
        |> WonkaHelpers.getEffectCleanup;
      // |> Firebase.Firestore.DocumentReference.collection(
      //      _,
      //      ~pathString="resumes",
      //    )
      // |> Firebase.Firestore.CollectionReference.getWithOptions(
      //      _,
      //      {source: Some("server")},
      //    )
      | _ => None
      }
    },
    [|authStatus|],
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
      <Link
        download=true
        disabled={
          switch (state) {
          | Success(_) => false
          | _ => true
          }
        }
        href="">
        {React.string("Download")}
      </Link>
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