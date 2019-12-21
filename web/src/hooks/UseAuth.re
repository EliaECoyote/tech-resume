// Docs:
// - firebaseUI auth demo: https://github.com/firebase/firebaseui-web/tree/master/demo
// - github auth management: https://firebase.google.com/docs/auth/web/github-auth#handle_the_sign-in_flow_with_the_firebase_sdk

type status =
  | Idle
  | Anonymous
  | Logged(Firebase.User.t);

let reducer = (~state, ~event) =>
  switch (state, event) {
  // anonymous login events handling
  | (Idle, Firebase.Auth.AnonymousLogin) => UseMachine.Valid(Anonymous)
  | (Logged(_), Firebase.Auth.AnonymousLogin) => UseMachine.Valid(Anonymous)
  // user login successful events handling
  | (Idle, Firebase.Auth.UserLoginSuccess(user)) =>
    Logged(user)->UseMachine.Valid
  | (Anonymous, Firebase.Auth.UserLoginSuccess(user)) =>
    Logged(user)->UseMachine.Valid
  // user login failure events handling
  | (_, Firebase.Auth.UserLoginFailed(error)) =>
    Js.log(error);
    UseMachine.Valid(Anonymous);
  | _ => UseMachine.Invalid(("useAuth invalid state", state, event))
  };

let hook = () => {
  let (state, sendEvent) = UseMachine.hook(~reducer, ~initialValue=Idle);

  React.useEffect1(
    () => {
      // instantiating the github auth provider as described in the docs
      // https://firebase.google.com/docs/auth/web/github-auth#handle_the_sign-in_flow_with_the_firebase_sdk
      let provider = Firebase.GithubAuthProvider.make();

      let subscription =
        Firebase.Auth.make()
        |> Firebase.Auth.authStateChange
        |> Wonka.subscribe((. event) => sendEvent(event));

      Some(subscription.unsubscribe);
    },
    [|sendEvent|],
  );

  state;
};