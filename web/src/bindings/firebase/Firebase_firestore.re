// Further types docs: https://github.com/firebase/firebase-js-sdk/blob/master/packages/firestore/index.ts

type firestoreT;

type documentDataT = Js.Json.t;
type dataConverterT('converterData);

type parsedSetDataT;
type maybeDocumentT;
type transactionT;
type documentKeyT;
type documentChangeT('data');
type snapshotMetadataT;
type documentSnapshotT('data);
type querySnapshotT('data);
type queryDocumentSnapshotT('data);

type documentReferenceT('data);
type collectionReferenceT('data);

type setOptionsT = {
  merge: option(bool),
  mergeFields: option(array(string)),
};
type getOptionsT = {source: option(string)};
type snapshotOptionsT = {serverTimestamps: option(string)};
type snapshotListenOptionsT = {includeMetadataChanges: option(bool)};

module DocumentKey = {
  type t = documentKeyT;

  [@bs.send] external hasCollectionId: (t, string) => bool = "hasCollectionId";

  [@bs.send] external isEqual: (t, t) => bool = "isEqual";
  // TODO continue implementing document key methods
};

module Transaction = {
  type t = transactionT;

  [@bs.send]
  external lookup:
    (t, ~keys: array(documentKeyT)) => Js.Promise.t(array(maybeDocumentT)) =
    "lookup";

  [@bs.send]
  external set: (t, ~key: documentKeyT, ~data: parsedSetDataT) => unit = "set";
  // TODO continue implementing transaction methods
};

module DocumentSnapshot = {
  type t('data) = documentSnapshotT('data);

  [@bs.send] external data: (t('data'), unit) => option('data) = "data";

  [@bs.send]
  external dataWithOptions: (t('data), snapshotOptionsT) => option('data) =
    "data";

  [@bs.send]
  external get: (t('data), ~fieldPath: string) => Js.Json.t = "get";

  [@bs.send]
  external getWithOptions:
    (t('data), ~fieldPath: string, ~options: snapshotOptionsT) => Js.Json.t =
    "get";

  [@bs.get] external get_id: t('data) => string = "id";

  [@bs.get] external get_ref: t('data) => documentReferenceT('data) = "ref";

  [@bs.get] external get_exists: t('data) => bool = "exists";

  [@bs.get] external get_metadata: t('data) => snapshotMetadataT = "metadata";

  [@bs.send]
  external isEqual: (t('data), ~other: t('data)) => bool = "isEqual";
};

module QueryDocumentSnapshot = {
  type t('data) = queryDocumentSnapshotT('data);

  external toQuerySnapshot: t('data) => querySnapshotT('data) = "%identity";

  [@bs.send] external data: t('data) => 'data = "data";

  [@bs.send]
  external dataWithOptions: (t('data), snapshotOptionsT) => 'data = "data";
};

module QuerySnapshot = {
  type t('data) = querySnapshotT('data);

  [@bs.send]
  external docChanges: t('data) => array(documentChangeT('data')) =
    "docChanges";

  [@bs.send]
  external docChangesWithOptions:
    (t('data), snapshotListenOptionsT) => array(documentChangeT('data')) =
    "docChanges";

  [@bs.send]
  external forEach:
    (t('data), (. queryDocumentSnapshotT('data)) => unit) => unit =
    "forEach";

  external isEqual: (t('data), ~other: t('data)) => bool = "isEqual";
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
    (t('data), getOptionsT) => Js.Promise.t(documentSnapshotT('data)) =
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

  // [@bs.get] external get(options?: GetOptions): Promise<QuerySnapshot<T>>;

  [@bs.send]
  external doc:
    (t('data), ~documentPath: string=?) => DocumentReference.t('data) =
    "doc";

  [@bs.send]
  external add:
    (t('data), ~data: 'data) => Js.Promise.t(DocumentReference.t('data)) =
    "add";

  [@bs.send]
  external get: t('data) => Js.Promise.t(querySnapshotT('data)) = "get";

  [@bs.send]
  external getWithOptions:
    (t('data), getOptionsT) => Js.Promise.t(querySnapshotT('data)) =
    "get";

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

[@bs.send]
external runTransaction:
  (firestoreT, transactionT => Js.Promise.t('a)) => Js.Promise.t('a) =
  "runTransaction";