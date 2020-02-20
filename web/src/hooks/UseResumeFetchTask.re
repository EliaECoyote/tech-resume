/**
 * hook that returns an AsyncTask that represents the status
 * of the resume loading request
 */
let hook = () => {
  open AsyncTask;
  let (authStatus, _) = React.useContext(AuthContext.context);
  let (state, send) = UseMachine.hook(~reducer, ~initialValue=Idle);
  let resumeDataService =
    Belt2.React.useSafeContext(ResumeDataServiceContext.context);

  // loads resume data upon *authStatus* change
  React.useEffect1(
    () =>
      switch (authStatus) {
      | Anonymous
      | Logged(_) =>
        send(LoadData);
        resumeDataService.loadResume(authStatus)
        |> Wonka.subscribe((. result) =>
             switch (result) {
             | Belt.Result.Ok(data) => send(LoadSuccess(data))
             | Belt.Result.Error(error) =>
               Js.Console.error(error);
               send(LoadFailed);
             }
           )
        |> WonkaHelpers.getEffectCleanup;
      | Idle => None
      },
    [|authStatus|],
  );

  state;
};