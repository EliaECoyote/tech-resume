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

  /**
   * encodes resume object into a JSON generic object
   */
  external encode: resumeDataT => Js.Json.t = "%identity";
};

module FirestoreResume = {
  /**
   * retrieves user-specific firestore resume documentRef source
   */
  let getDocumentRef = (db, user: Firebase.Auth.User.t) => {
    let uid = user.uid;
    let collectionPath = {j|users/$uid/resumesDetails|j};
    let resumesDetailsRef =
      Firebase.Firestore.collection(db, ~collectionPath);
    let resumesDetailsQuery =
      Firebase.Firestore.CollectionReference.asQuery(resumesDetailsRef);
    Firebase.Firestore.Query.limit(resumesDetailsQuery, ~limit=1)
    |> Firebase.Firestore.Query.get
    |> WonkaHelpers.fromPromise
    |> Wonka.switchMap((. snapshot) =>
         switch (snapshot) {
         | Belt.Result.Ok(snapshot) =>
           Firebase.Firestore.QuerySnapshot.get_empty(snapshot)
             // when no resumesDetails can be found, create one on the fly
             ? Firebase.Firestore.CollectionReference.add(
                 resumesDetailsRef,
                 ~data=ResumeJson.encode(initialResumeData),
               )
               |> WonkaHelpers.fromPromise
             // otherwise just return the existing one snapshot
             : Firebase.Firestore.QuerySnapshot.get_docs(snapshot)
               |> Array.get(_, 0)
               |> Wonka.fromValue
               |> Wonka.map((. snapshot) =>
                    snapshot
                    |> Firebase.Firestore.QueryDocumentSnapshot.toDocumentSnapshot
                    |> Firebase.Firestore.DocumentSnapshot.get_ref
                    |> (ref_ => Belt.Result.Ok(ref_))
                  )
         | Belt.Result.Error(error) =>
           Wonka.fromValue(Belt.Result.Error(error))
         }
       );
  };

  /**
   * retrieves the resume object, given a snapshot
   */
  let getData = snapshot =>
    switch (snapshot) {
    | Belt.Result.Ok(snapshot) =>
      // retrieve data from documentRef's snapshot
      let resume =
        Firebase.Firestore.DocumentSnapshot.data(snapshot)
        // decode retrieved data
        |> Belt.Option.flatMap(_, ResumeJson.decode);
      switch (resume) {
      | Some(resume) => Belt.Result.Ok(resume)
      | None =>
        let error =
          Js2.Exn.makeError(
            "Unexpected error: could not retrieve resume object",
          );
        Belt.Result.Error(error);
      };
    | Belt.Result.Error(error) => Belt.Result.Error(error)
    };
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
  let db = Firebase.Firestore.make();
  let resumeDocumentRef = ref(None);

  /**
   * loads storage resume data
   */
  let loadStorageResume = () => {
    let template =
      Dom.Storage.localStorage |> Dom.Storage.getItem("template");
    let theme = Dom.Storage.localStorage |> Dom.Storage.getItem("theme");
    let result =
      switch (template, theme) {
      | (Some(template), Some(theme)) => Belt.Result.Ok({template, theme})
      | _ =>
        Dom.Storage.localStorage
        |> Dom.Storage.setItem("template", initialResumeData.template);
        Dom.Storage.localStorage
        |> Dom.Storage.setItem("theme", initialResumeData.theme);
        Belt.Result.Ok(initialResumeData);
      };
    Wonka.fromValue(result);
  };

  /**
   * loads firestore resume data
   */
  let loadFirestoreResume = user => {
    let sharedResumeDocRefSource =
      FirestoreResume.getDocumentRef(db, user)
      |> WonkaHelpers.Operators.shareReplay(1);

    let resumeDataSource =
      sharedResumeDocRefSource
      |> Wonka.switchMap((. documentRef) =>
           switch (documentRef) {
           | Belt.Result.Ok(documentRef) =>
             // retrieve snapshot from documentRef
             Firebase.Firestore.DocumentReference.get(documentRef)
             |> WonkaHelpers.fromPromise
             |> Wonka.map((. snapshot) => FirestoreResume.getData(snapshot))
           | Belt.Result.Error(error) =>
             Wonka.fromValue(Belt.Result.Error(error))
           }
         );

    let docRefSubscription =
      sharedResumeDocRefSource
      |> Wonka.subscribe((. documentRef) =>
           switch (documentRef) {
           | Belt.Result.Ok(documentRef) =>
             resumeDocumentRef := Some(documentRef)
           | Belt.Result.Error(error) =>
             resumeDocumentRef := None;
             Js.Console.error2("document ref not loaded successfully", error);
           }
         );

    resumeDataSource |> Wonka.onEnd((.) => docRefSubscription.unsubscribe());
  };

  /**
   * saves resume data on firestore
   */
  let saveFirestoreResume = (data: resumeDataT) =>
    switch (resumeDocumentRef^) {
    | Some(documentRef) =>
      ResumeJson.encode(data)
      |> Firebase.Firestore.DocumentReference.set(documentRef, ~value=_)
      |> WonkaHelpers.fromPromise
    | None =>
      let error = Js2.Exn.makeError("documentRef not available");
      Wonka.fromValue(Belt.Result.Error(error));
    };

  /**
   * saves resume data on storage
   */
  let saveStorageResume = (data: resumeDataT) => {
    Dom.Storage.localStorage |> Dom.Storage.setItem("template", data.template);
    Dom.Storage.localStorage |> Dom.Storage.setItem("theme", data.theme);
    Wonka.fromValue @@ Belt.Result.Ok();
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
      Wonka.fromValue @@
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
      Wonka.fromValue @@
      Belt.Result.Error(Js2.Exn.makeError("Cannot load: user not found"))
    };

  {saveResume, loadResume};
};