module Styles = {
  open Css;
  let authWidget = style([]);
  let loggedUserGrid =
    style([
      display(`grid),
      height(`rem(4.0)),
      gridTemplateRows([`percent(50.0), `percent(50.0)]),
      gridTemplateColumns([`fr(1.0), `rem(4.0)]),
      gridTemplateAreas(`areas(["username avatar", "cta avatar"])),
      gridColumnGap(`rem(1.0)),
    ]);
  let username = style([gridArea(`ident("username")), margin(`px(0))]);
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

module type FirebaseUIType = {include (module type of FirebaseUI);};

let dynamicImportFirebaseUI: unit => Js.Promise.t(module FirebaseUIType) = [%bs.raw
  {| () => import("../../bindings/FirebaseUI.js") |}
];

let loginUIElementID = "auth";

let startFirebaseUIWidget = () =>
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
         signInFlow: Some("popup"),
         signInOptions: Some([|Firebase.GithubAuthProvider.providerId|]),
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
let make =
    (~authStatus: UseAuth.status, ~signOut: unit => Wonka.Types.subscriptionT) => {
  React.useEffect1(
    () => {
      switch (authStatus) {
      | UseAuth.Anonymous =>
        let subscription = startFirebaseUIWidget();
        Some(subscription.unsubscribe);
      | _ => None
      }
    },
    [|authStatus|],
  );

  let onLogoutCtaClick = _ => {
    let _ = signOut();
    ();
  };

  switch (authStatus) {
  | UseAuth.Idle => ReasonReact.null
  | UseAuth.Logged(user) =>
    <div className=Styles.loggedUserGrid>
      <p className=Styles.username>
        <b>
          {Belt.Option.getWithDefault(user.displayName, "Unknown user")
           |> React.string}
        </b>
      </p>
      <Button
        className=Styles.cta size=Controls.Small onClick=onLogoutCtaClick>
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