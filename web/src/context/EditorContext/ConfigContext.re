type state = {theme: Themes.t};

type actions =
  | ChangeTheme(Themes.t);

let defaultDispatcher: actions => unit = _ => ();
let initialState: state = {theme: Themes.Standard};

let context = React.createContext((initialState, defaultDispatcher));

let reducer = (_, action) =>
  switch (action) {
  | ChangeTheme(value) => {theme: value}
  };

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
  <div> <Provider value=(state, dispatch)> children </Provider> </div>;
};