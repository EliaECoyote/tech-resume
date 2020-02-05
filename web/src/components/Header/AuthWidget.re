module Styles = {
  open Css;
  let authWidget = style([]);
  let loggedUserGrid =
    style([
      display(`grid),
      height(`rem(3.0)),
      gridTemplateRows([`percent(50.0), `percent(50.0)]),
      gridTemplateColumns([`fr(1.0), `rem(3.0)]),
      gridTemplateAreas(`areas(["username avatar", "cta avatar"])),
      gridColumnGap(`rem(1.0)),
    ]);
  let username =
    [
      gridArea(`ident("username")),
      textOverflow(`ellipsis),
      whiteSpace(`nowrap),
      overflow(`hidden),
    ]
    |> (styles => List.concat([styles, TextStyles.textEllipsis]))
    |> (styles => List.concat([styles, TextStyles.xsFont]))
    |> style;
  let cta = style([gridArea(`ident("cta"))]);
  let thumb =
    style([
      gridArea(`ident("avatar")),
      width(`percent(100.0)),
      height(`percent(100.0)),
      objectFit(`cover),
      borderRadius(`px(8)),
    ]);
};

let loginUIElementID = "auth";

let startFirebaseUIWidget = () =>
  Firebase.UI.importDinamically()
  |> Wonka.fromPromise
  // FirebaseUI library dynamic import source
  |> Wonka.subscribe((. module FirebaseUI: Firebase.UI.FirebaseUiType) => {
       let callbacks: FirebaseUI.callbacks = {
         // signInSuccessWithAuthResult callback returns false in
         // order to avoid redirects after a successful login as
         // described in
         // https://github.com/firebase/firebaseui-web/blob/master/README.md#signinsuccesswithauthresultauthresult-redirecturl
         signInSuccessWithAuthResult:
           Some((~authResult as _, ~redirectUrl as _=?, ()) => false),
         signInFailure: None,
         uiShown: None,
       };
       let uiConfig: FirebaseUI.config = {
         acUiConfig: None,
         autoUpgradeAnonymousUsers: None,
         callbacks: Some(callbacks),
         credentialHelper: None,
         popupMode: None,
         queryParameterForSignInSuccessUrl: Some("login-success"),
         queryParameterForWidgetMode: Some("mode"),
         signInFlow: Some("popup"),
         signInOptions: Some([|Firebase.Auth.GithubAuthProvider.providerId|]),
         signInSuccessUrl: None,
         siteName: None,
         tosUrl: None,
         privacyPolicyUrl: None,
         widgetUrl: None,
       };
       let id = "#" ++ loginUIElementID;
       // starts actual firebase UI widget
       Firebase.Auth.make()
       |> FirebaseUI.authUI(_, ())
       |> FirebaseUI.start(_, `Id(id), uiConfig);
     });

[@react.component]
let make = (~authStatus: UseAuth.status, ~signOut: unit => unit) => {
  React.useEffect1(
    () => {
      switch (authStatus) {
      | UseAuth.Anonymous =>
        startFirebaseUIWidget() |> WonkaHelpers.getEffectCleanup
      | _ => None
      }
    },
    [|authStatus|],
  );

  switch (authStatus) {
  | UseAuth.Idle => ReasonReact.null
  | UseAuth.Logged(user) =>
    <div className=Styles.loggedUserGrid>
      <span className=Styles.username>
        {Belt.Option.getWithDefault(user.displayName, "Unknown user")
         |> React.string}
      </span>
      <Button
        className=Styles.cta size=Controls.Small onClick={_ => signOut()}>
        {React.string("Log out")}
      </Button>
      // user mini thumb rendering
      {switch (user.photoURL) {
       | Some(url) => <img src=url className=Styles.thumb />
       | None => ReasonReact.null
       }}
    </div>
  | UseAuth.Anonymous => <div id=loginUIElementID />
  };
};