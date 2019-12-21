module Styles = {
  open Css;
  let authWidget = style([width(`px(300))]);
  let headerTitle = style([]);
};

module type FirebaseUIType = {include (module type of FirebaseUI);};

let dynamicImportFirebaseUI: unit => Js.Promise.t(module FirebaseUIType) = [%bs.raw
  {| () => import("../../bindings/FirebaseUI.js") |}
];

let loginUIElementID = "auth";

[@react.component]
let make = (~authStatus: UseAuth.status) => {
  React.useEffect1(
    () => {
      switch (authStatus) {
      | UseAuth.Anonymous =>
        let subscription =
          dynamicImportFirebaseUI()
          |> Wonka.fromPromise
          // FirebaseUI library dynamic import source
          |> Wonka.subscribe((. module FirebaseUI: FirebaseUIType) => {
               let uiConfig: FirebaseUI.config = {
                 acUiConfig: None,
                 autoUpgradeAnonymousUsers: None,
                 callbacks: None,
                 credentialHelper: None,
                 popupMode: None,
                 queryParameterForSignInSuccessUrl: Some("login-success"),
                 queryParameterForWidgetMode: Some("mode"),
                 signInFlow: None,
                 signInOptions:
                   Some([|Firebase.GithubAuthProvider.providerId|]),
                 signInSuccessUrl: None,
                 siteName: None,
                 tosUrl: None,
                 privacyPolicyUrl: None,
                 widgetUrl: None,
               };
               let id = "#" ++ loginUIElementID;
               // starts firebase UI widget
               Firebase.Auth.make()
               |> FirebaseUI.authUI(_, ())
               |> FirebaseUI.start(_, `Id(id), uiConfig);
             });
        Some(subscription.unsubscribe);
      | _ => None
      }
    },
    [|authStatus|],
  );

  <div className=Styles.authWidget>
    {switch (authStatus) {
     | UseAuth.Idle => React.string("resolving authentication")
     | UseAuth.Logged(user) =>
       React.string(
         "Logged in as "
         ++ Belt.Option.getWithDefault(user.displayName, "test"),
       )
     | UseAuth.Anonymous => <div id=loginUIElementID />
     }}
  </div>;
};