open Wonka.Types;
include Wonka_operators;

/**
 * uncurried filter
 */
let filter = callback => Wonka.filter((. value) => callback(value));

/**
 * uncurried debounce
 */
let debounce = callback => Wonka.debounce((. value) => callback(value));

/**
 * uncurried concatMap
 */
let concatMap = callback => Wonka.concatMap((. value) => callback(value));

/**
 * uncurried map
 */
let map = callback => Wonka.map((. value) => callback(value));

/**
 * uncurried tap
 */
let tap = callback => Wonka.tap((. value) => callback(value));

/**
 * uncurried scan
 */
let scan = callback => Wonka.scan((. acc, value) => callback(acc, value));

/**
 * uncurried mergeMap
 */
let mergeMap = callback => Wonka.mergeMap((. value) => callback(value));

/**
 * uncurried switchmap
 */
let switchMap = callback => Wonka.switchMap((. value) => callback(value));

/**
 * uncurried onPush
 */
let onPush = callback => Wonka.onPush((. value) => callback(value));

/**
 * uncurried onPush
 */
let onEnd = callback => Wonka.onEnd((.) => callback());

/**
 * uncurried onStart
 */
let onStart = callback => Wonka.onStart((.) => callback());

/**
 * maps *result* source events *only* if result is Ok
 */
let mapOk = callback =>
  map(result =>
    switch (result) {
    | Belt.Result.Ok(value) => callback(value)
    | Belt.Result.Error(error) => Belt.Result.Error(error)
    }
  );

/**
 * switchMaps *result* source events *only* if result is Ok
 */
let switchMapOk = callback =>
  switchMap(result =>
    switch (result) {
    | Belt.Result.Ok(value) => callback(value)
    | Belt.Result.Error(error) => Wonka.fromValue(Belt.Result.Error(error))
    }
  );

/**
 * switchMaps tuple *result*(s) source events *only* if result(s) is Ok
 */
let switchMapOk2 = callback =>
  switchMap(((result1, result2)) =>
    switch (result1, result2) {
    | (Belt.Result.Ok(value1), Belt.Result.Ok(value2)) =>
      callback((value1, value2))
    | (Belt.Result.Error(error), _) =>
      Wonka.fromValue(Belt.Result.Error(error))
    | (_, Belt.Result.Error(error)) =>
      Wonka.fromValue(Belt.Result.Error(error))
    }
  );

type shareReplayStateT('a) = {replaySubject: subjectT('a)};

/**
 * share source and replay specified number of emissions on subscription
 */
let shareReplay = (bufferSize: int, source: sourceT('a)): sourceT('a) => {
  let state = {
    replaySubject: Belt2_wonka_sources.makeReplaySubject(bufferSize),
  };

  let _subscription =
    source
    |> onPush(state.replaySubject.next)
    |> onEnd(state.replaySubject.complete)
    |> Wonka.publish;

  state.replaySubject.source;
};

/**
 * Logs errors on Wonka {Belt.Result.t} sources
 */
let tapLogError = (~message: string, source: sourceT(Belt.Result.t('a, 'b))) =>
  source
  |> tap(result =>
       switch (result) {
       | Belt.Result.Error(error) => Js.Console.error2(message, error)
       | _ => ()
       }
     );

/**
 * applies Wonka *combine* operator to an array
 * of sources
 */
let combineArray = sourcesArray =>
  Belt.Array.reduce(sourcesArray, Wonka.fromValue([||]), (source, accSource) =>
    Wonka.combine(accSource, source)
    |> map(((result, resultsArray)) =>
         Array.append(resultsArray, [|result|])
       )
  );

/**
 * applies Wonka *combine* operator to a list
 * of sources
 */
let combineList = sourcesList =>
  sourcesList |> Array.of_list |> combineArray |> map(Array.to_list);