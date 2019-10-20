type colors = {
  primaryColor: string,
  backgroundColor: string,
  accent: string,
};

type state = {
  theme: Themes.t,
  colors,
};

type actions =
  | ChangeTheme(Themes.t);

let getColors = theme =>
  switch (theme) {
  | Themes.Black => {
      primaryColor: "#fff",
      backgroundColor: "#000",
      accent: "#f38518",
    }
  | Themes.Dark => {
      primaryColor: "#d4d4d4",
      backgroundColor: "#1e1e1e",
      accent: "#c6c6c6",
    }
  | Themes.Standard => {
      primaryColor: "#000",
      backgroundColor: "#fffffe",
      accent: "#c6c6c6",
    }
  };

let defaultTheme = Themes.Standard;
let defaultDispatcher: actions => unit = _ => ();
let initialState: state = {
  theme: defaultTheme,
  colors: getColors(defaultTheme),
};

let context = React.createContext((initialState, defaultDispatcher));

let reducer = (_, action) =>
  switch (action) {
  | ChangeTheme(value) => {theme: value, colors: getColors(value)}
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