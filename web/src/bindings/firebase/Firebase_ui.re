// Further types docs: https://github.com/firebase/firebaseui-web/blob/master/types/index.d.ts
type t;

type authError = {
  code: string,
  message: string,
};

type callbacks = {
  signInSuccessWithAuthResult:
    option((~authResult: Js.t(unit), ~redirectUrl: string=?, unit) => bool),
  signInFailure: option(option((~error: authError) => Js.Promise.t(unit))),
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
  emailLinkSignIn: option(unit => Firebase_auth.ActionCodeSettings.t),
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
  signInOptions: option(array(Firebase_auth.AuthProvider.providerId)),
  signInSuccessUrl: option(string),
  siteName: option(string),
  tosUrl: option(urlKind),
  privacyPolicyUrl: option(urlKind),
  widgetUrl: option(string),
};

[@bs.new] [@bs.module "firebaseui"] [@bs.scope "auth"]
external authUI: (Firebase_auth.t, ~appId: string=?, unit) => t = "AuthUI";
let authUI = authUI;
[@bs.send] external disableAutoSignIn: t => unit = "disableAutoSignIn";
[@bs.send]
external start:
  (t, [@bs.unwrap] [ | `Id(string) | `El(Dom.element)], config) => unit =
  "start";
let start = start;
[@bs.send] external setConfig: (t, config) => unit = "setConfig";
[@bs.send] external signIn: t => unit = "signIn";
[@bs.send] external reset: t => unit = "reset";
[@bs.send] external delete: t => Js.Promise.t(unit) = "delete";
[@bs.send] external isPendingRedirect: t => bool = "isPendingRedirect";