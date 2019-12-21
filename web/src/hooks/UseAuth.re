// Docs:
// - firebaseUI auth demo: https://github.com/firebase/firebaseui-web/tree/master/demo
// - github auth management: https://firebase.google.com/docs/auth/web/github-auth#handle_the_sign-in_flow_with_the_firebase_sdk

module type FirebaseUIType = {include (module type of FirebaseUI);};

let dynamicImportFirebaseUI: unit => Js.Promise.t(module FirebaseUIType) = [%bs.raw
  {| () => import("../bindings/FirebaseUI.js") |}
];

type status =
  | Idle
  | Anonymous
  | Logged(Firebase.User.t);

let reducer = (~state, ~event) =>
  switch (state, event) {
  // anonymous login events handling
  | (Idle, Firebase.Auth.AnonymousLogin) => Anonymous->UseMachine.Valid
  | (Logged(_), Firebase.Auth.AnonymousLogin) => Anonymous->UseMachine.Valid
  // user login successful events handling
  | (Idle, Firebase.Auth.UserLoginSuccess(user)) =>
    Logged(user)->UseMachine.Valid
  | (Anonymous, Firebase.Auth.UserLoginSuccess(user)) =>
    Logged(user)->UseMachine.Valid
  // user login failure events handling
  | (_, Firebase.Auth.UserLoginFailed(error)) => Anonymous->UseMachine.Valid
  | _ => UseMachine.Invalid(("useAuth invalid state", state, event))
  };

let hook = () => {
  let (state, sendEvent) = UseMachine.hook(~reducer, ~initialValue=Idle);

  React.useEffect1(
    () => {
      // FirebaseUI library dynamic import source
      let subscription =
        dynamicImportFirebaseUI()
        |> Wonka.fromPromise
        |> Wonka.mergeMap((. module FirebaseUI: FirebaseUIType) => {
             // instantiating the github auth provider as described in the docs
             // https://firebase.google.com/docs/auth/web/github-auth#handle_the_sign-in_flow_with_the_firebase_sdk
             let provider = Firebase.GithubAuthProvider.make();
             let auth = Firebase.Auth.make();
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
             // starts firebase UI widget
             FirebaseUI.authUI(~auth, ())
             |> FirebaseUI.start(_, `Id("#auth"), uiConfig);
             // returns authentication change source
             Firebase.Auth.authStateChange(auth);
           })
        |> Wonka.subscribe((. event) => sendEvent(event));

      Some(subscription.unsubscribe);
    },
    [|sendEvent|],
  );
};