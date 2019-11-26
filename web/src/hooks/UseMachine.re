/**
 * describes the outcome of the machine reducer
 * fn.
 * The Invalid variant value describes the case
 * where an undesired event is received
 * (cannot receive the event while being in
 * current state).
 * The Ignored variant value describes the case
 * where we just want to not take action after
 * receiving a specific event.
 */
type outcome('state, 'reason) =
  | Valid('state)
  | Invalid('reason)
  | Ignored;

type fsmReducer('state, 'event, 'reason) =
  (~state: 'state, ~event: 'event) => outcome('state, 'reason);

/**
 * uses the reducer parameter to return only
 * actual machine state values, and not *outcomes*.
 */
let getMextState = (reducer, state, event) => {
  switch (reducer(~state, ~event)) {
  | Valid(value) => value
  | Ignored => state
  | Invalid(reason) =>
    // right now the only action taken when an invalid
    // event occurs is loggin. In the future this can
    // be tracked in different ways
    Js.log(reason);
    state;
  };
};

type t('state, 'event, 'reason) =
  (~reducer: fsmReducer('state, 'event, 'reason), ~initialValue: 'state) =>
  ('state, 'event => unit);

/**
 * react hook that manages a FSM.
 * There's no static chart here - the chart is
 * actually contained in the reducer function,
 * by using the combination of tuple, variants
 * and pattern matching reasonML features
 */
let hook: t('state, 'event, 'reason) =
  (~reducer, ~initialValue) => {
    let enhancedReducerRef = React.useRef(None);

    let getReducerWrapper = () => {
      // creates the reducerWrapper with *reducerWrapperFactory*,
      // but *only once*!
      switch (enhancedReducerRef->React.Ref.current) {
      | Some(value) => value
      | None =>
        // uses partial application & currying to generate
        // a reducer that returns actual machine state values
        let enhancedReducer = getMextState(reducer);
        enhancedReducerRef->React.Ref.setCurrent(Some(enhancedReducer));
        enhancedReducer;
      };
    };

    React.useReducer(getReducerWrapper(), initialValue);
  };