// Further types docs: https://github.com/firebase/firebase-js-sdk/blob/master/packages/firestore/index.ts
[%bs.raw {| require("firebase/firestore") |}];

type firestoreT;

type documentDataT = Js.Json.t;
type dataConverterT('converterData);
type documentSnapshotT('data);

type documentReferenceT('data);
type collectionReferenceT('data);

type setOptionsT = {
  merge: option(bool),
  mergeFields: option(array(string)),
};

module DocumentReference = {
  type t('data) = documentReferenceT('data);
  type updateDataT = Js.Json.t;

  [@bs.get] external get_id: t('data) => string = "id";

  [@bs.get]
  external get_parent:
    t('data) => Js.Nullable.t(collectionReferenceT('data)) =
    "parent";

  [@bs.get] external get_path: t('data) => string = "path";

  [@bs.send]
  external withConverter:
    (t('data), ~converter: dataConverterT('converterData)) =>
    t('converterData) =
    "add";

  [@bs.send]
  external get: t('data) => Js.Promise.t(documentSnapshotT('data)) = "get";

  [@bs.send]
  external getWithOptions:
    (t('data), [@bs.string] [ | `default | `server | `cache]) =>
    Js.Promise.t(documentSnapshotT('data)) =
    "get";

  [@bs.send] external delete: t('data) => Js.Promise.t(unit) = "delete";

  [@bs.send]
  external set: (t('data), ~value: documentDataT) => Js.Promise.t(unit) =
    "set";
  [@bs.send]
  external setWithOptions:
    (t('data), ~value: documentDataT, ~options: setOptionsT) =>
    Js.Promise.t(unit) =
    "set";

  [@bs.send]
  external updateValue: (t('data), ~value: updateDataT) => Js.Promise.t(unit) =
    "update";

  [@bs.send]
  external updateFieldData:
    (t('data), ~field: string, ~value: updateDataT) => Js.Promise.t(unit) =
    "update";

  [@bs.send]
  external isEqual: (t('data), ~other: t('data)) => bool = "isEqual";

  [@bs.send]
  external collection:
    (t('data), ~pathString: string) => collectionReferenceT(documentDataT) =
    "collection";
  // TODO: add bindings for the following fns
  //   onSnapshot(
  //     observer: PartialObserver<firestore.DocumentSnapshot<T>>
  //   ): Unsubscribe;
  //   onSnapshot(
  //     options: firestore.SnapshotListenOptions,
  //     observer: PartialObserver<firestore.DocumentSnapshot<T>>
  //   ): Unsubscribe;
  //   onSnapshot(
  //     onNext: NextFn<firestore.DocumentSnapshot<T>>,
  //     onError?: ErrorFn,
  //     onCompletion?: CompleteFn
  //   ): Unsubscribe;
  //   onSnapshot(
  //     options: firestore.SnapshotListenOptions,
  //     onNext: NextFn<firestore.DocumentSnapshot<T>>,
  //     onError?: ErrorFn,
  //     onCompletion?: CompleteFn
  //   ): Unsubscribe;
  //   onSnapshot(...args: unknown[]): Unsubscribe
};

module CollectionReference = {
  type t('data) = collectionReferenceT('data);

  [@bs.get] external get_id: t('data) => string = "id";

  [@bs.get]
  external get_parent: t('data) => Js.Nullable.t(DocumentReference.t('data)) =
    "parent";

  [@bs.get] external get_path: t('data) => string = "path";

  [@bs.send]
  external doc:
    (t('data), ~documentPath: option(string)=?) =>
    DocumentReference.t('data) =
    "doc";

  [@bs.send]
  external add: (t('data), ~data: 'data) => DocumentReference.t('data) =
    "add";

  [@bs.send]
  external isEqual: (t('data), ~other: t('data)) => bool = "isEqual";

  [@bs.send]
  external withConverter:
    (t('dataA), ~converter: dataConverterT('dataB)) => t('dataB) =
    "withConverter";
};

[@bs.module "firebase/app"] external make: unit => firestoreT = "firestore";

[@bs.send]
external collection:
  (firestoreT, ~collectionPath: string) => collectionReferenceT(documentDataT) =
  "collection";