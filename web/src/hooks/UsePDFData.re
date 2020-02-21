type dataT = {template: string};

let hook = () => {
  let (data, setData) = React.useState(() => None);
  let (state, sendEvent) =
    UseMachine.hook(~reducer=AsyncTask.reducer, ~initialValue=Idle);

  React.useEffect1(
    () =>
      Belt.Option.flatMap(
        data,
        data => {
          sendEvent(LoadData);
          Apis.fetchPdfData(~md=data.template)
          |> Wonka.subscribe((. value) =>
               switch (value) {
               | HttpClient.Ok(data) => sendEvent(LoadSuccess(data))
               | HttpClient.Failure
               | HttpClient.FailureCode(_) => sendEvent(LoadFailed)
               }
             )
          |> Belt2.Wonka.getEffectCleanup;
        },
      ),
    [|data|],
  );

  let fetch = React.useCallback0(md => setData(_ => Some({template: md})));
  (state, fetch);
};