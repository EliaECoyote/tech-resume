type json;

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

module AuthProvider = {
  type t;
  type providerId;
};

module GithubAuthProvider = {
  type t = AuthProvider.t;

  [@bs.new] [@bs.module "firebase/app"] [@bs.scope "auth"]
  external make: unit => t = "GithubAuthProvider";

  [@bs.send] external addScope: (t, string) => t = "addScope";

  [@bs.module "firebase/app"] [@bs.scope ("auth", "GithubAuthProvider")]
  external providerId: AuthProvider.providerId = "PROVIDER_ID";
};

module AuthCredential = {
  type t = {
    providerId: AuthProvider.providerId,
    signInMethod: string,
  };

  [@bs.send] external toJSON: (t, unit) => json = "toJSON";
};

module ApplicationVerifier = {
  type t = {
    [@bs.as "type"]
    type_: string,
  };

  [@bs.send] external verify: unit => Js.Promise.t(string) = "verify";
};

// Further types docs: https://github.com/firebase/firebase-js-sdk/blob/master/packages/auth-types/index.d.ts
module User = {
  type userMetadata = {
    creationTime: option(string),
    lastSignInTime: option(string),
  };

  type userInfo = {
    displayName: option(string),
    email: option(string),
    phoneNumber: option(string),
    photoURL: option(string),
    providerId: AuthProvider.providerId,
    uid: string,
  };

  type t = {
    displayName: option(string),
    email: option(string),
    phoneNumber: option(string),
    photoURL: option(string),
    providerId: AuthProvider.providerId,
    uid: string,
    emailVerified: bool,
    isAnonymous: bool,
    metadata: userMetadata,
    providerData: array(Js.Nullable.t(userInfo)),
    refreshToken: string,
    tenantId: Js.Nullable.t(string),
  };
};

module AdditionalUserInfo = {
  type t = {
    isNewUser: bool,
    providerId: AuthProvider.providerId,
    profile: option(Js.Dict.t(unit)),
    username: option(string),
  };
};

module UserCredential = {
  type t = {
    additionalUserInfo: option(AdditionalUserInfo.t),
    credential: Js.Nullable.t(AuthCredential.t),
    operationType: option(string),
    user: Js.Nullable.t(User.t),
  };
};

module ConfirmationResult = {
  type t = {verificationId: string};

  [@bs.send]
  external confirm:
    (t, ~verificationCode: string) => Js.Promise.t(UserCredential.t) =
    "confirm";
};

module ActionCodeSettings = {
  type iOS = {bundleId: string};
  type android = {
    packageName: string,
    installApp: option(bool),
    minimumVersion: option(string),
  };

  type t = {
    url: string,
    handleCodeInApp: option(bool),
    iOS: option(iOS),
    android: option(android),
    dynamicLinkDomain: option(string),
  };
};

module UserUtils = {
  type t = User.t;

  type idTokenResult = {
    token: string,
    expirationTime: string,
    authTime: string,
    issuedAtTime: string,
    claims: Js.Dict.t(string),
    signInProvider: option(string),
  };

  type userCredential = {
    additionalUserInfo: option(AdditionalUserInfo.t),
    credential: Js.Nullable.t(AuthCredential.t),
    operationType: option(string),
    user: Js.Nullable.t(User.t),
  };

  type profile = {
    displayName: option(string),
    photoURL: option(string),
  };

  [@bs.send] external delete: (t, unit) => Js.Promise.t(unit) = "delete";
  [@bs.send]
  external getIdTokenResult:
    (t, ~forceRefresh: bool=?, unit) => Js.Promise.t(idTokenResult) =
    "getIdTokenResult";
  [@bs.send]
  external getIdToken:
    (t, ~forceRefresh: bool=?, unit) => Js.Promise.t(string) =
    "getIdToken";
  [@bs.send]
  external linkAndRetrieveDataWithCredential:
    (t, ~credential: AuthCredential.t) => Js.Promise.t(userCredential) =
    "linkAndRetrieveDataWithCredential";
  [@bs.send]
  external linkWithCredential:
    (t, ~credential: AuthCredential.t) => Js.Promise.t(userCredential) =
    "linkWithCredential";
  [@bs.send]
  external linkWithPhoneNumber:
    (t, ~phoneNumber: string, ~applicationVerifier: ApplicationVerifier.t) =>
    Js.Promise.t(ConfirmationResult.t) =
    "linkWithPhoneNumber";
  [@bs.send]
  external linkWithPopup:
    (t, ~provider: AuthProvider.t) => Js.Promise.t(userCredential) =
    "linkWithPopup";
  [@bs.send]
  external linkWithRedirect:
    (t, ~provider: AuthProvider.t) => Js.Promise.t(unit) =
    "linkWithRedirect";
  [@bs.send]
  external reauthenticateAndRetrieveDataWithCredential:
    (t, ~credential: AuthCredential.t) => Js.Promise.t(userCredential) =
    "reauthenticateAndRetrieveDataWithCredential";
  [@bs.send]
  external reauthenticateWithCredential:
    (t, ~credential: AuthCredential.t) => Js.Promise.t(userCredential) =
    "reauthenticateWithCredential";
  [@bs.send]
  external reauthenticateWithPhoneNumber:
    (t, ~phoneNumber: string, ~applicationVerifier: AuthCredential.t) =>
    Js.Promise.t(ConfirmationResult.t) =
    "reauthenticateWithPhoneNumber";
  [@bs.send]
  external reauthenticateWithPopup:
    (t, ~provider: AuthProvider.t) => Js.Promise.t(userCredential) =
    "reauthenticateWithPopup";
  [@bs.send]
  external reauthenticateWithRedirect:
    (t, ~provider: AuthProvider.t) => Js.Promise.t(unit) =
    "reauthenticateWithRedirect";
  [@bs.send]
  external unlink:
    (t, ~providerId: AuthProvider.providerId) => Js.Promise.t(t) =
    "unlink";
  [@bs.send]
  external updateEmail: (t, ~newEmail: string) => Js.Promise.t(unit) =
    "updateEmail";
  [@bs.send]
  external updatePassword: (t, ~newPassword: string) => Js.Promise.t(unit) =
    "updatePassword";
  [@bs.send]
  external updatePhoneNumber:
    (t, ~phoneCredential: AuthCredential.t) => Js.Promise.t(unit) =
    "updatePhoneNumber";
  [@bs.send]
  external updateProfile: (t, ~profile: profile) => Js.Promise.t(unit) =
    "updateProfile";
  [@bs.send] external reload: (t, unit) => Js.Promise.t(unit) = "reload";
  [@bs.send]
  external sendEmailVerification:
    (t, ~actionCodeSettings: ActionCodeSettings.t=?) => Js.Promise.t(unit) =
    "sendEmailVerification";
};

module Auth = {
  type t;

  [@bs.module "firebase/app"] external make: unit => t = "auth";

  type nextOrObserver = Js.Nullable.t(User.t) => unit;
  type error = Js.Exn.t => unit;
  type unsubscribe = unit => unit;
  type events =
    | AnonymousLogin
    | UserLoginSuccess(User.t)
    | UserLoginFailed(Js.Exn.t);

  [@bs.send]
  external getRedirectResult: t => Js.Promise.t(string) = "getRedirectResult";
  [@bs.send]
  external signInWithRedirect: (t, AuthProvider.t) => unit =
    "signInWithRedirect";
  [@bs.send]
  external onAuthStateChanged:
    (
      t,
      ~nextOrObserver: nextOrObserver,
      ~error: error=?,
      ~completed: unsubscribe=?,
      unit
    ) =>
    unsubscribe =
    "onAuthStateChanged";
  let getAuthEventFromUser = (user: Js.Nullable.t(User.t)) =>
    switch (user->Js.Nullable.toOption) {
    | Some(value) => UserLoginSuccess(value)
    | None => AnonymousLogin
    };
  // onAuthStateChanged wonka wrapper
  let authStateChange = auth =>
    Wonka.make((. observer: Wonka.Types.observerT(events)) => {
      let unsubscribe =
        onAuthStateChanged(
          auth,
          ~nextOrObserver=user => getAuthEventFromUser(user) |> observer.next,
          ~error=error => UserLoginFailed(error) |> observer.next,
          (),
        );
      (.) => unsubscribe();
    });
};