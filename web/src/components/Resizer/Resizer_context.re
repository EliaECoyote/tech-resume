type state = {
  left: float,
  right: float,
  resizing: bool,
  resizerWidth: float,
  resizerOffset: float,
};

type actions =
  | UpdateResizerWidth(float)
  | UpdateResizerOffset(float)
  | UpdateSectionsFlexGrow((float, float))
  | StartResizing
  | EndResizing;

let reducer = (state, action) =>
  switch (action) {
  | UpdateResizerWidth(value) => {...state, resizerWidth: value}
  | UpdateResizerOffset(value) => {...state, resizerOffset: value}
  | UpdateSectionsFlexGrow((left, right)) => {...state, left, right}
  | StartResizing => {...state, resizing: true}
  | EndResizing => {...state, resizing: false}
  };

let defaultDispatcher: actions => unit = _ => ();
let initialState: state = {
  left: 1.0,
  right: 1.0,
  resizing: false,
  resizerOffset: 0.0,
  resizerWidth: 0.0,
};
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