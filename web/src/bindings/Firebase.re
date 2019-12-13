module GithubAuthProvider = {
  type t;

  [@bs.new] [@bs.module "firebase"] [@bs.scope "auth"]
  external make: unit => t = "GithubAuthProvider";

  [@bs.send] external addScope: (t, string) => t = "addScope";

  [@bs.module "firebase"] [@bs.scope "auth"] [@bs.scope "GithubAuthProvider"]
  external providerId: string = "PROVIDER_ID";
};

module Auth = {
  type t;

  [@bs.module "firebase"] external make: unit => t = "auth";

  [@bs.send]
  external getRedirectResult: t => Js.Promise.t(string) = "getRedirectResult";

  [@bs.send]
  external signInWithRedirect: (t, GithubAuthProvider.t) => unit =
    "signInWithRedirect";
};

module UI = {
  // Further types docs: https://github.com/firebase/firebaseui-web/blob/master/types/index.d.ts
  type t;

  type element = [ | `Str(string) | `El(Dom.element)];

  [@bs.deriving abstract]
  type authError = {
    code: string,
    message: string,
  };

  [@bs.deriving abstract]
  type callbacks = {
    [@bs.optional]
    signInSuccessWithAuthResult:
      (~authResult: Js.t(unit), ~redirectUrl: string=?) => bool,
    [@bs.optional]
    signInFailure: (~error: authError) => Js.Promise.t(unit),
    [@bs.optional]
    uiShown: unit => unit,
  };

  type credentialHelperType = string;

  [@bs.deriving abstract]
  type oAuthSignInOption = {
    buttonColor: string,
    iconUrl: array(string),
    [@bs.optional]
    providerName: string,
    [@bs.optional]
    scopes: array(string),
    [@bs.optional]
    customParameters: Js.Dict.t(string),
    [@bs.optional]
    loginHintKey: string,
  };

  [@bs.deriving abstract]
  type iOS = {bundleId: string};
  [@bs.deriving abstract]
  type android = {
    packageName: string,
    [@bs.optional]
    installApp: bool,
    [@bs.optional]
    minimumVersion: string,
  };

  [@bs.deriving abstract]
  type actionCodeSettings = {
    url: string,
    [@bs.optional]
    handleCodeInApp: bool,
    [@bs.optional]
    iOS,
    [@bs.optional]
    android,
    [@bs.optional]
    dynamicLinkDomain: string,
  };

  [@bs.deriving abstract]
  type emailSignInOption = {
    [@bs.optional]
    forceSameDevice: bool,
    [@bs.optional]
    requireDisplayName: bool,
    [@bs.optional]
    signInMethod: string,
    [@bs.optional]
    emailLinkSignIn: unit => actionCodeSettings,
  };

  [@bs.deriving abstract]
  type recaptchaParameters = {
    [@bs.optional] [@bs.as "type"]
    type_: string,
    [@bs.optional]
    size: string,
    [@bs.optional]
    badge: string,
  };

  [@bs.deriving abstract]
  type phoneSignInOption = {
    [@bs.optional]
    recaptchaParameters,
    [@bs.optional]
    defaultCountry: string,
    [@bs.optional]
    defaultNationalNumber: string,
    [@bs.optional]
    loginHint: string,
    [@bs.optional]
    whitelistedCountries: array(string),
    [@bs.optional]
    blacklistedCountries: array(string),
  };

  [@bs.deriving abstract]
  type samlSignInOption = {
    buttonColor: string,
    iconUrl: string,
    [@bs.optional]
    providerName: string,
  };

  [@bs.deriving abstract]
  type federatedSignInOption = {
    [@bs.optional]
    authMethod: string,
    [@bs.optional]
    clientId: string,
    [@bs.optional]
    scopes: array(string),
    [@bs.optional]
    customParameters: Js.Dict.t(string),
  };

  [@bs.deriving abstract]
  type oidcSignInOption = {
    buttonColor: string,
    iconUrl: string,
    [@bs.optional]
    providerName: string,
    [@bs.optional]
    customParameters: Js.Dict.t(string),
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

  [@bs.deriving abstract]
  type config = {
    // [@bs.optional]
    // acUiConfig?: object;
    [@bs.optional]
    autoUpgradeAnonymousUsers: bool,
    [@bs.optional]
    callbacks,
    [@bs.optional]
    credentialHelper: credentialHelperType,
    [@bs.optional]
    popupMode: bool,
    [@bs.optional]
    queryParameterForSignInSuccessUrl: string,
    [@bs.optional]
    queryParameterForWidgetMode: string,
    [@bs.optional]
    signInFlow: string,
    [@bs.optional]
    signInOptions,
    [@bs.optional]
    signInSuccessUrl: string,
    [@bs.optional]
    siteName: string,
    [@bs.optional]
    tosUrl: urlKind,
    [@bs.optional]
    privacyPolicyUrl: urlKind,
    [@bs.optional]
    widgetUrl: string,
  };
  // start(element: string | Element, config: firebaseui.auth.Config): void;
  [@bs.send] external start: (t, element, config) => unit = "start";
};

module AuthUI = {
  [@bs.new] [@bs.module "firebaseui"] [@bs.scope "auth"]
  external authUI: Auth.t => UI.t = "AuthUI";
};