type state = {
  left: float,
  right: float,
  resizerWidth: float,
};

type actions =
  | UpdateResizerWidth(float)
  | UpdateSectionsSizes((float, float));

let reducer = (state, action) =>
  switch (action) {
  | UpdateResizerWidth(value) => {...state, resizerWidth: value}
  | UpdateSectionsSizes((left, right)) => {...state, left, right}
  };

let defaultDispatcher: actions => unit = _ => ();
let initialState: state = {left: 1.0, right: 1.0, resizerWidth: 0.0};
let context = React.createContext((initialState, defaultDispatcher));

module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
  let make = React.Context.provider(context);
};

[@react.component]
let make = (~children) => {
  let (state, dispatch) = React.useReducer(reducer, initialState);
  <Provider value=(state, dispatch)> children </Provider>;
};