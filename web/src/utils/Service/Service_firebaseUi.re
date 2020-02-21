type firebaseUiServiceT = {
  getFirebaseAuthUi: unit => option(Firebase_ui.t),
  startFirebaseUi: unit => Wonka_types.subscriptionT,
};

let make = (elementId: string): firebaseUiServiceT => {
  let firebaseAuthUi = ref(None);

  let getFirebaseAuthUi = () => firebaseAuthUi^;

  let startFirebaseUi = () =>
    Firebase.UI.importDinamically()
    |> XWonka.fromPromise
    |> XWonka.subscribe((module FirebaseUI: Firebase.UI.FirebaseUiType) => {
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
           signInOptions:
             Some([|Firebase.Auth.GithubAuthProvider.providerId|]),
           signInSuccessUrl: None,
           siteName: None,
           tosUrl: None,
           privacyPolicyUrl: None,
           widgetUrl: None,
         };
         let id = "#" ++ elementId;
         let authUi =
           switch (firebaseAuthUi^) {
           | Some(firebaseAuthUi) => firebaseAuthUi
           | None =>
             let newFirebaseAuthUi =
               Firebase.Auth.make() |> FirebaseUI.authUI(_, ()) |> Obj.magic;
             firebaseAuthUi := Some(newFirebaseAuthUi);
             newFirebaseAuthUi;
           };
         // starts actual firebase UI widget
         FirebaseUI.start(Obj.magic(authUi), `Id(id), uiConfig);
       });

  {startFirebaseUi, getFirebaseAuthUi};
};