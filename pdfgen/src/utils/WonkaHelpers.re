external promiseErrorToJsExn: Js.Promise.error => Js.Exn.t = "%identity";

/**
 * converts a promise to a Wonka source
 * with result
 * when promise resolves or rejects, the source
 * will push *next* & *complete*.
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