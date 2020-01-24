external promiseErrorToJsExn: Js.Promise.error => Js.Exn.t = "%identity";

/**
 * Converts a promise to a Wonka *result* source.
 * Upon promise resolution / rejection, the source
 * will push *next* & *complete* events
 */
let fromPromise = (promise: Js.Promise.t('a)) => {
  Wonka.make(
    (. observer: Wonka.Types.observerT(Belt.Result.t('a, Js.Exn.t))) => {
    let cancelled = ref(false);
    let _ =
      promise
      |> Js.Promise.then_(value =>
           Belt.Result.Ok(value) |> Js.Promise.resolve
         )
      |> Js.Promise.catch(error =>
           promiseErrorToJsExn(error)
           |> (error => Belt.Result.Error(error))
           |> Js.Promise.resolve
         )
      |> Js.Promise.then_(value => {
           if (! cancelled^) {
             observer.next(value);
             observer.complete();
           };
           Js.Promise.resolve();
         });
    (.) => cancelled := true;
  });
};

/**
 * HOC that, given a subscription, returns an optional
 * unsubscribtion fn.
 * This is useful for easier unsubscription inside
 * React **useEffect** hooks
 */
let getEffectCleanup = (subscription: Wonka_types.subscriptionT) =>
  Some(subscription.unsubscribe);

module Result = {
  let logResultError = (message, result) =>
    switch (result) {
    | Belt.Result.Error(error) => Js.Console.error2(message, error)
    | _ => ()
    };

  /**
   * Logs errors on Wonka {Belt.Result.t} sources
   */
  let tapLogError =
      (~message: string, source: Wonka.Types.sourceT(Belt.Result.t('a, 'b))) =>
    source |> Wonka.tap((. result) => logResultError(message, result));
};

let combineArray = sourcesArray =>
  Belt.Array.reduce(sourcesArray, Wonka.fromValue([||]), (source, accSource) =>
    Wonka.combine(accSource, source)
    |> Wonka.map((. (result, resultsArray)) =>
         Array.append(resultsArray, [|result|])
       )
  );

let combineList = sourcesList =>
  sourcesList
  |> Array.of_list
  |> combineArray
  |> Wonka.map((. sourcesArray) => Array.to_list(sourcesArray));