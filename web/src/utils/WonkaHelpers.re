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
           Js2.Exn.promiseExnToJsExn(error)
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
 * HOF that, given a subscription, returns an optional
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

module Sources = {
  open Wonka.Types;

  type replaySubjectState('a) = {
    mutable values: Rebel.Array.t('a),
    mutable sinks: Rebel.Array.t(sinkT('a)),
    mutable ended: bool,
  };

  let makeReplaySubject = (bufferSize: int): subjectT('a) => {
    let state: replaySubjectState('a) = {
      values: Rebel.Array.makeEmpty(),
      sinks: Rebel.Array.makeEmpty(),
      ended: false,
    };

    let source = sink => {
      let isClosed = ref(false);
      state.sinks = Rebel.Array.append(state.sinks, sink);
      sink(.
        Start(
          (. signal) =>
            switch (signal) {
            | Close =>
              state.sinks = Rebel.Array.filter(state.sinks, x => x !== sink);
              isClosed := true;
            | _ => ()
            },
        ),
      );
      Rebel.Array.forEach(state.values, value =>
        if (! isClosed^) {
          sink(. Push(value));
        }
      );
    };

    let rebelPrepend = (array, value) =>
      Rebel.Array.concat(Rebel.Array.make(1, value), array);

    let next = value =>
      if (!state.ended) {
        let newValues = ref(rebelPrepend(state.values, value));
        if (Rebel.Array.size(state.values) > bufferSize) {
          newValues :=
            Rebel.Array.slice(
              newValues^,
              ~start=0,
              ~end_=Rebel.Array.size(newValues^) - bufferSize,
            );
        };
        state.values = newValues^;
        Rebel.Array.forEach(state.sinks, sink => sink(. Push(value)));
      };

    let complete = () =>
      if (!state.ended) {
        state.ended = true;
        state.values = Rebel.Array.makeEmpty();
        Rebel.Array.forEach(state.sinks, sink => sink(. End));
      };

    {source, next, complete};
  };
};

module Operators = {
  open Wonka.Types;

  type shareReplayStateT('a) = {replaySubject: subjectT('a)};

  let shareReplay = (bufferSize: int, source: sourceT('a)): sourceT('a) => {
    let state = {replaySubject: Sources.makeReplaySubject(bufferSize)};

    let _subscription =
      source
      |> Wonka.onPush((. value) => state.replaySubject.next(value))
      |> Wonka.onEnd((.) => state.replaySubject.complete())
      |> Wonka.publish;

    state.replaySubject.source;
  };
};