type json;

module AuthProvider = {
  type t;
};

module GithubAuthProvider = {
  type t = AuthProvider.t;

  [@bs.new] [@bs.module "firebase"] [@bs.scope "auth"]
  external make: unit => t = "GithubAuthProvider";

  [@bs.send] external addScope: (t, string) => t = "addScope";

  [@bs.module "firebase"] [@bs.scope "auth"] [@bs.scope "GithubAuthProvider"]
  external providerId: string = "PROVIDER_ID";
};

module AuthCredential = {
  type t = {
    providerId: string,
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
    providerId: string,
    uid: string,
  };

  type t = {
    displayName: option(string),
    email: option(string),
    phoneNumber: option(string),
    photoURL: option(string),
    providerId: string,
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
    providerId: string,
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
  external unlink: (t, ~providerId: string) => Js.Promise.t(t) = "unlink";
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

  type unsubscribe = unit => unit;

  [@bs.module "firebase"] external make: unit => t = "auth";

  [@bs.send]
  external getRedirectResult: t => Js.Promise.t(string) = "getRedirectResult";
  [@bs.send]
  external signInWithRedirect: (t, AuthProvider.t) => unit =
    "signInWithRedirect";
  [@bs.send]
  external onAuthStateChanged:
    (
      ~nextOrObserver: Js.Nullable.t(User.t) => unit,
      ~error: option(Js.Exn.t) => unit,
      ~completed: unsubscribe
    ) =>
    unsubscribe =
    "onAuthStateChanged";
};

// Further types docs: https://github.com/firebase/firebaseui-web/blob/master/types/index.d.ts
module UI = {
  type t;

  type element = [ | `Str(string) | `El(Dom.element)];

  type authError = {
    code: string,
    message: string,
  };

  type callbacks = {
    signInSuccessWithAuthResult:
      option((~authResult: Js.t(unit), ~redirectUrl: string=?) => bool),
    signInFailure:
      option(option((~error: authError) => Js.Promise.t(unit))),
    uiShown: option(unit => unit),
  };

  type credentialHelperType = string;

  type oAuthSignInOption = {
    buttonColor: string,
    iconUrl: array(string),
    providerName: option(string),
    scopes: option(array(string)),
    customParameters: option(Js.Dict.t(string)),
    loginHintKey: option(string),
  };

  type emailSignInOption = {
    forceSameDevice: option(bool),
    requireDisplayName: option(bool),
    signInMethod: option(string),
    emailLinkSignIn: option(unit => ActionCodeSettings.t),
  };

  type recaptchaParameters = {
    [@bs.as "type"]
    type_: option(string),
    size: option(string),
    badge: option(string),
  };

  type phoneSignInOption = {
    recaptchaParameters: option(recaptchaParameters),
    defaultCountry: option(string),
    defaultNationalNumber: option(string),
    loginHint: option(string),
    whitelistedCountries: option(array(string)),
    blacklistedCountries: option(array(string)),
  };

  type samlSignInOption = {
    buttonColor: string,
    iconUrl: string,
    providerName: option(string),
  };

  type federatedSignInOption = {
    authMethod: option(string),
    clientId: option(string),
    scopes: option(array(string)),
    customParameters: option(Js.Dict.t(string)),
  };

  type oidcSignInOption = {
    buttonColor: string,
    iconUrl: string,
    providerName: option(string),
    customParameters: option(Js.Dict.t(string)),
  };

  type signInOptions = [
    | `Str(string)
    | `Federated(federatedSignInOption)
    | `Email(emailSignInOption)
    | `Phone(phoneSignInOption)
    | `Saml(samlSignInOption)
    | `OAuth(oAuthSignInOption)
    | `Oidc(oidcSignInOption)
  ];
  type urlKind = [ | `Callback(unit => unit) | `Str(string)];

  type config = {
    acUiConfig: option(Js.t(unit)),
    autoUpgradeAnonymousUsers: option(bool),
    callbacks: option(callbacks),
    credentialHelper: option(credentialHelperType),
    popupMode: option(bool),
    queryParameterForSignInSuccessUrl: option(string),
    queryParameterForWidgetMode: option(string),
    signInFlow: option(string),
    signInOptions: option(signInOptions),
    signInSuccessUrl: option(string),
    siteName: option(string),
    tosUrl: option(urlKind),
    privacyPolicyUrl: option(urlKind),
    widgetUrl: option(string),
  };

  [@bs.send] external start: (t, element, config) => unit = "start";
};

module AuthUI = {
  [@bs.new] [@bs.module "firebaseui"] [@bs.scope "auth"]
  external authUI: Auth.t => UI.t = "AuthUI";
};