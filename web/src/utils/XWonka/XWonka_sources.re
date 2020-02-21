open Wonka.Types;
include Wonka_sources;
include WonkaJs;

/**
 * uncurried make
 */
let make = callback =>
  make((. observer) => {
    let teardown = callback(observer);
    (.) => teardown();
  });

/**
 * Converts a promise to a Wonka *result* source.
 * Upon promise resolution / rejection, the source
 * will push *next* & *complete* events
 */
let fromPromiseSafe = (promise: Js.Promise.t('a)) => {
  Wonka.make((. observer: observerT(Belt.Result.t('a, Js.Exn.t))) => {
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

type replaySubjectStateT('a) = {
  mutable values: Rebel.Array.t('a),
  mutable sinks: Rebel.Array.t(sinkT('a)),
  mutable ended: bool,
};

/**
 * creates a replay subject with a specific bufferSize.
 * Upon subscription, it will reemit the last *bufferSize*
 * events to the subscriber
 */
let makeReplaySubject = (bufferSize: int): subjectT('a) => {
  let state: replaySubjectStateT('a) = {
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