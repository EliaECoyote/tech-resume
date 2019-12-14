// Docs:
// - firebaseUI auth demo: https://github.com/firebase/firebaseui-web/tree/master/demo
// - github auth management: https://firebase.google.com/docs/auth/web/github-auth#handle_the_sign-in_flow_with_the_firebase_sdk

let uiConfig: Firebase.UI.config = {
  acUiConfig: None,
  autoUpgradeAnonymousUsers: None,
  callbacks: None,
  credentialHelper: None,
  popupMode: None,
  queryParameterForSignInSuccessUrl: Some("login-success"),
  queryParameterForWidgetMode: Some("mode"),
  signInFlow: None,
  signInOptions: Some([|Firebase.GithubAuthProvider.providerId|]),
  signInSuccessUrl: None,
  siteName: None,
  tosUrl: None,
  privacyPolicyUrl: None,
  widgetUrl: None,
};

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
      // instantiating the github auth provider as described in the docs
      // https://firebase.google.com/docs/auth/web/github-auth#handle_the_sign-in_flow_with_the_firebase_sdk
      let provider = Firebase.GithubAuthProvider.make();

      let auth = Firebase.Auth.make();
      Firebase.UI.authUI(~auth, ())
      |> Firebase.UI.start(_, `Id("#auth"), uiConfig);

      // setting up firebase user current state source$
      let subscription =
        auth
        |> Firebase.Auth.authStateChange
        |> Wonka.subscribe((. event) => sendEvent(event));

      Some(subscription.unsubscribe);
    },
    [|sendEvent|],
  );
};