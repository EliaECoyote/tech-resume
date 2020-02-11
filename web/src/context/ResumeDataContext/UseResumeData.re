type resumeDataT = {
  template: string,
  theme: string,
};

let initialResumeData = {
  template: DefaultTemplates.dwightTemplate,
  theme: "default",
};

let getStandardResumeObject = () => {
  let resume = Js.Dict.empty();
  Js.Dict.set(
    resume,
    "template",
    Js.Json.string(initialResumeData.template),
  );
  Js.Dict.set(resume, "theme", Js.Json.string(initialResumeData.theme));
  Js.Json.object_(resume);
};

let decodeResumeObject = data =>
  data
  |> Js.Json.decodeObject
  |> Belt.Option.flatMap(
       _,
       dic => {
         let template =
           Js.Dict.get(dic, "template")
           |> Belt.Option.flatMap(_, Js.Json.decodeString);
         let theme =
           Js.Dict.get(dic, "theme")
           |> Belt.Option.flatMap(_, Js.Json.decodeString);
         switch (template, theme) {
         | (Some(template), Some(theme)) => Some({template, theme})
         | _ => None
         };
       },
     );

let hook = () => {
  open AsyncTask;
  let (state, send) = UseMachine.hook(~reducer, ~initialValue=Idle);
  let (authStatus, _) = React.useContext(AuthContext.context);

  React.useEffect1(
    () => {
      switch (authStatus) {
      | Logged(user) =>
        send(LoadData);
        let db = Firebase.Firestore.make();
        let uid = user.uid;
        let collectionPath = {j|users/$uid/resumesDetails|j};
        let resumesDetailsRef =
          Firebase.Firestore.collection(db, ~collectionPath);
        let resumesDetailsQuery =
          Firebase.Firestore.CollectionReference.asQuery(resumesDetailsRef);
        // fetch resumesDetails collection
        Firebase.Firestore.Query.limit(resumesDetailsQuery, ~limit=1)
        |> Firebase.Firestore.Query.get
        |> WonkaHelpers.fromPromise
        |> Wonka.switchMap((. snapshot)
             // TODO: use Firestore transactions https://firebase.google.com/docs/firestore/manage-data/transactions
             // in order to make sure that the future "add resume title & id to user document" will be
             // atomic
             =>
               switch (snapshot) {
               | Belt.Result.Ok(snapshot) =>
                 Firebase.Firestore.QuerySnapshot.get_empty(snapshot)
                   // when no resumesDetails can be found, create one
                   // on the fly
                   ? Firebase.Firestore.CollectionReference.add(
                       resumesDetailsRef,
                       ~data=getStandardResumeObject(),
                     )
                     |> WonkaHelpers.fromPromise
                     |> Wonka.switchMap((. documentRef) =>
                          switch (documentRef) {
                          | Belt.Result.Ok(documentRef) =>
                            Firebase.Firestore.DocumentReference.get(
                              documentRef,
                            )
                            |> WonkaHelpers.fromPromise
                          | Belt.Result.Error(error) =>
                            Belt.Result.Error(error) |> Wonka.fromValue
                          }
                        )
                   // otherwise just return the resumesDetails snapshot
                   : Firebase.Firestore.QuerySnapshot.get_docs(snapshot)
                     |> Array.get(_, 0)
                     |> Wonka.fromValue
                     |> Wonka.map((. snapshot) =>
                          snapshot
                          |> Firebase.Firestore.QueryDocumentSnapshot.toDocumentSnapshot
                          |> (snapshot => Belt.Result.Ok(snapshot))
                        )
               | Belt.Result.Error(error) =>
                 Wonka.fromValue(Belt.Result.Error(error))
               }
             )
        |> Wonka.subscribe((. snapshot) =>
             switch (snapshot) {
             | Belt.Result.Ok(snapshot) =>
               // retrieve and decode the firestore snapshot data
               snapshot
               |> Firebase.Firestore.DocumentSnapshot.data
               |> Belt.Option.flatMap(_, decodeResumeObject)
               |> (
                 data =>
                   switch (data) {
                   | Some(data) => send(LoadSuccess(data))
                   | None =>
                     Js.Console.error((
                       "Firestore data decode failure",
                       data,
                     ));
                     send(LoadFailed);
                   }
               )
             | Belt.Result.Error(error) =>
               Js.Console.error(error);
               send(LoadFailed);
             }
           )
        |> WonkaHelpers.getEffectCleanup;
      | Anonymous =>
        // load template & theme from localStorage -
        // when resume data isn't available, set it
        // on the fly in localStorage
        send(LoadData);
        let template =
          Dom.Storage.localStorage |> Dom.Storage.getItem("template");
        let theme = Dom.Storage.localStorage |> Dom.Storage.getItem("theme");
        switch (template, theme) {
        | (Some(template), Some(theme)) =>
          send(LoadSuccess({template, theme}))
        | _ =>
          Dom.Storage.localStorage
          |> Dom.Storage.setItem("template", initialResumeData.template);
          Dom.Storage.localStorage
          |> Dom.Storage.setItem("theme", initialResumeData.theme);
          send(LoadSuccess(initialResumeData));
        };
        None;
      | _ => None
      }
    },
    [|authStatus|],
  );

  let saveData = React.useCallback((data: resumeDataT) => {Js.log(data)});

  (state, saveData);
};