module type FirestoreType = (module type of Firebase.Firestore);

type resumeDataT = {
  template: string,
  theme: string,
};

let initialResumeData = {
  template: DefaultTemplates.dwightTemplate,
  theme: "default",
};

module ResumeJson = {
  /**
   * decodes JSON resume object into a typed object.
   * Returns None in case of a decoding failure
   */
  let decode = data =>
    data
    |> Js.Json.decodeObject
    |> Belt2.Option.flatMap(dic => {
         let template =
           Js.Dict.get(dic, "template")
           |> Belt2.Option.flatMap(Js.Json.decodeString);
         let theme =
           Js.Dict.get(dic, "theme")
           |> Belt2.Option.flatMap(Js.Json.decodeString);
         switch (template, theme) {
         | (Some(template), Some(theme)) => Some({template, theme})
         | _ => None
         };
       });

  /**
   * encodes resume object into a JSON generic object
   */
  external encode: resumeDataT => Js.Json.t = "%identity";
};

/**
 * shared source that emits the firebase module
 */
let firestoreModuleSource:
  XWonka.Types.sourceT(Belt.Result.t(module FirestoreType, Js.Exn.t)) =
  Belt2.Import.import("../../bindings/firebase/Firebase_firestore.bs.js")
  |> XWonka.fromPromiseSafe
  |> XWonka.shareReplay(1);

module FirestoreResume = {
  /**
   * retrieves user-specific firestore resume documentRef source
   */
  let getDocumentRef = (db, user: Firebase.Auth.User.t) =>
    firestoreModuleSource
    |> XWonka.switchMapOk((module Firestore: FirestoreType) => {
         let uid = user.uid;
         let collectionPath = {j|users/$uid/resumesDetails|j};
         let resumesDetailsRef = Firestore.collection(db, ~collectionPath);
         let resumesDetailsQuery =
           Firestore.CollectionReference.asQuery(resumesDetailsRef);
         Firestore.Query.limit(resumesDetailsQuery, ~limit=1)
         |> Firestore.Query.get
         |> XWonka.fromPromiseSafe
         |> XWonka.switchMapOk(snapshot =>
              Firestore.QuerySnapshot.get_empty(snapshot)
                // when no resumesDetails can be found, create one on the fly
                ? Firestore.CollectionReference.add(
                    resumesDetailsRef,
                    ~data=ResumeJson.encode(initialResumeData),
                  )
                  |> XWonka.fromPromiseSafe
                // otherwise just return the existing one snapshot
                : Firestore.QuerySnapshot.get_docs(snapshot)
                  |> Array.get(_, 0)
                  |> XWonka.fromValue
                  |> XWonka.map(snapshot =>
                       snapshot
                       |> Firestore.QueryDocumentSnapshot.toDocumentSnapshot
                       |> Firestore.DocumentSnapshot.get_ref
                       |> (ref_ => Belt.Result.Ok(ref_))
                     )
            );
       });

  /**
   * retrieves the resume object, given a snapshot
   */
  let getData = snapshot =>
    firestoreModuleSource
    |> XWonka.mapOk((module Firestore: FirestoreType) => {
         // retrieve data from documentRef's snapshot
         let resume =
           Firestore.DocumentSnapshot.data(snapshot)
           // decode retrieved data
           |> Belt2.Option.flatMap(ResumeJson.decode);
         switch (resume) {
         | Some(resume) => Belt.Result.Ok(resume)
         | None =>
           let error =
             Js2.Exn.makeError(
               "Unexpected error: could not retrieve resume object",
             );
           Belt.Result.Error(error);
         };
       });
};

type resumeDataServiceT = {
  saveResume:
    (UseAuth.status, resumeDataT) =>
    Wonka_types.sourceT(Belt.Result.t(unit, Js.Exn.t)),
  loadResume:
    UseAuth.status =>
    Wonka_types.sourceT(Belt.Result.t(resumeDataT, Js.Exn.t)),
};

let make = (): resumeDataServiceT => {
  /**
   * shared source that emits the firestore-generated db instance
   */
  let dbSource =
    firestoreModuleSource
    |> XWonka.map(result =>
         Belt.Result.map(result, (module Firestore: FirestoreType) =>
           Firestore.make()
         )
       )
    |> XWonka.shareReplay(1);

  let resumeDocumentRef = ref(None);

  /**
   * loads storage resume data
   */
  let loadStorageResume = () =>
    firestoreModuleSource
    |> XWonka.take(1)
    |> XWonka.switchMapOk((module Firestore: FirestoreType) => {
         let template =
           Dom.Storage.localStorage |> Dom.Storage.getItem("template");
         let theme = Dom.Storage.localStorage |> Dom.Storage.getItem("theme");
         let result =
           switch (template, theme) {
           | (Some(template), Some(theme)) =>
             Belt.Result.Ok({template, theme})
           | _ =>
             Dom.Storage.localStorage
             |> Dom.Storage.setItem("template", initialResumeData.template);
             Dom.Storage.localStorage
             |> Dom.Storage.setItem("theme", initialResumeData.theme);
             Belt.Result.Ok(initialResumeData);
           };
         XWonka.fromValue(result);
       });

  /**
   * loads firestore resume data
   */
  let loadFirestoreResume = user => {
    let sharedResumeDocRefSource =
      dbSource
      |> XWonka.switchMapOk(FirestoreResume.getDocumentRef(_, user))
      |> XWonka.shareReplay(1);

    let resumeDataSource =
      XWonka.combine(firestoreModuleSource, sharedResumeDocRefSource)
      |> XWonka.switchMapOk2(
           ((module Firestore: FirestoreType, documentRef)) =>
           Firestore.DocumentReference.get(documentRef)
           |> XWonka.fromPromiseSafe
           |> XWonka.tap(value => Js.log(value))
           |> XWonka.switchMapOk(FirestoreResume.getData)
         );

    let docRefSubscription =
      sharedResumeDocRefSource
      |> XWonka.subscribe(documentRef =>
           switch (documentRef) {
           | Belt.Result.Ok(documentRef) =>
             resumeDocumentRef := Some(documentRef)
           | Belt.Result.Error(error) =>
             resumeDocumentRef := None;
             Js.Console.error2("document ref not loaded successfully", error);
           }
         );

    resumeDataSource |> XWonka.onEnd(docRefSubscription.unsubscribe);
  };

  /**
   * saves resume data on firestore
   */
  let saveFirestoreResume = (data: resumeDataT) =>
    switch (resumeDocumentRef^) {
    | Some(documentRef) =>
      firestoreModuleSource
      |> XWonka.switchMapOk((module Firestore: FirestoreType) =>
           ResumeJson.encode(data)
           |> Firestore.DocumentReference.set(documentRef, ~value=_)
           |> XWonka.fromPromiseSafe
         )
    | None =>
      let error = Js2.Exn.makeError("documentRef not available");
      XWonka.fromValue(Belt.Result.Error(error));
    };

  /**
   * saves resume data on storage
   */
  let saveStorageResume = (data: resumeDataT) => {
    Dom.Storage.localStorage |> Dom.Storage.setItem("template", data.template);
    Dom.Storage.localStorage |> Dom.Storage.setItem("theme", data.theme);
    XWonka.fromValue @@ Belt.Result.Ok();
  };

  /**
   * saves resume on firestore / storage depending on
   * the user auth
   */
  let saveResume = (user, data: resumeDataT) =>
    switch (user) {
    | UseAuth.Logged(_user) => saveFirestoreResume(data)
    | UseAuth.Anonymous => saveStorageResume(data)
    | UseAuth.Idle =>
      XWonka.fromValue @@
      Belt.Result.Error(Js2.Exn.makeError("Cannot save: user not found"))
    };

  /**
   * loads resume from firestore / storage depending on
   * the user auth
   */
  let loadResume = user =>
    switch (user) {
    | UseAuth.Logged(user) => loadFirestoreResume(user)
    | UseAuth.Anonymous => loadStorageResume()
    | UseAuth.Idle =>
      XWonka.fromValue @@
      Belt.Result.Error(Js2.Exn.makeError("Cannot load: user not found"))
    };

  {saveResume, loadResume};
};