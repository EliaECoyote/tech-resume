type firebaseAppConfig = {
  name: option(string),
  automaticDataCollectionEnabled: option(bool),
};

type firebaseOptions = {
  apiKey: option(string),
  authDomain: option(string),
  databaseURL: option(string),
  projectId: option(string),
  storageBucket: option(string),
  messagingSenderId: option(string),
  appId: option(string),
  measurementId: option(string),
};

[@bs.module "firebase/app"]
external initializeApp: (~firebaseOptions: firebaseOptions) => unit =
  "initializeApp";

[@bs.module "firebase/app"]
external initializeAppByName:
  (~firebaseOptions: firebaseOptions, ~name: string, unit) => unit =
  "initializeApp";

[@bs.module "firebase/app"]
external initializeAppByConfig:
  (~firebaseOptions: firebaseOptions, ~config: firebaseAppConfig, unit) => unit =
  "initializeApp";

[@bs.module "firebase/app"] external analytics: unit => unit = "analytics";

module Auth = Firebase_auth;
module Firestore = Firebase_firestore;
module UI = {
  module type FirebaseUiType = {include (module type of Firebase_ui);};

  let importDinamically: unit => Js.Promise.t(module FirebaseUiType) = [%bs.raw
    {| () => import(
      /* webpackChunkName: "firebase_ui" */
      "./Firebase_ui.bs.js"
    ) |}
  ];
};