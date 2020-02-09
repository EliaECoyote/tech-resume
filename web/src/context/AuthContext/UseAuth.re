// Docs:
// - firebaseUI auth demo: https://github.com/firebase/firebaseui-web/tree/master/demo
// - github auth management: https://firebase.google.com/docs/auth/web/github-auth#handle_the_sign-in_flow_with_the_firebase_sdk

type status =
  | Idle
  | Anonymous
  | Logged(Firebase.Auth.User.t);

type events =
  | UserNotFound
  | AnonymousLogin
  | UserLoginSuccess(Firebase.Auth.User.t)
  | UserLoginFailed(Js.Exn.t);

let getAuthEventFromUser = (user: Js.Nullable.t(Firebase.Auth.User.t)) =>
  switch (user |> Js.Nullable.toOption) {
  | Some(value) => UserLoginSuccess(value)
  | None => AnonymousLogin
  };

let makeAuthStateChangeSource = auth =>
  Wonka.make((. observer: Wonka.Types.observerT(events)) => {
    let unsubscribe =
      Firebase.Auth.onAuthStateChanged(
        auth,
        ~nextOrObserver=user => getAuthEventFromUser(user) |> observer.next,
        ~error=error => UserLoginFailed(error) |> observer.next,
        (),
      );
    (.) => unsubscribe();
  });

let reducer = (~state, ~event) =>
  switch (state, event) {
  // anonymous login events handling
  | (Idle, AnonymousLogin) => UseMachine.Valid(Anonymous)
  | (Logged(_), AnonymousLogin) => UseMachine.Valid(Anonymous)
  // user login successful events handling
  | (Idle, UserLoginSuccess(user)) => UseMachine.Valid(Logged(user))
  | (Anonymous, UserLoginSuccess(user)) => UseMachine.Valid(Logged(user))
  // user login failure events handling
  | (_, UserLoginFailed(error)) =>
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
      let _provider = Firebase.Auth.GithubAuthProvider.make();
      Firebase.Auth.make()
      |> makeAuthStateChangeSource
      |> Wonka.subscribe((. event) => sendEvent(event))
      |> WonkaHelpers.getEffectCleanup;
    },
    [|sendEvent|],
  );

  let signOut =
    React.useCallback1(
      () =>
        Firebase.Auth.make()
        |> Firebase.Auth.signOut
        |> WonkaHelpers.fromPromise
        |> Wonka.take(1)
        |> Wonka.onPush((. result) =>
             switch (result) {
             | Belt.Result.Ok(_) => ()
             | Belt.Result.Error(error) =>
               Js.Console.error(error);
               sendEvent(AnonymousLogin);
             }
           )
        |> Wonka.publish
        |> (_ => ()),
      [|sendEvent|],
    );

  (state, signOut);
};