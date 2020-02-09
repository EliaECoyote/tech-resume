type colors = {
  text: Css.Types.Color.t,
  text2: Css.Types.Color.t,
  primary: Css.Types.Color.t,
  background: Css.Types.Color.t,
  surface: Css.Types.Color.t,
  accent: Css.Types.Color.t,
  error: Css.Types.Color.t,
};

type state = {
  theme: Themes.t,
  colors,
};

type actions =
  | ChangeTheme(Themes.t);

let getColors = theme =>
  switch (theme) {
  | Themes.Dark => {
      text: `hex("fff"),
      text2: `hex("000"),
      primary: `hex("03dac6"),
      background: `hex("282c33"),
      surface: `hex("282c33c7"),
      accent: `hex("03dac6"),
      error: `hex("cf6679"),
    }
  | Themes.Standard => {
      text: `hex("000"),
      text2: `hex("fff"),
      primary: `hex("006064"),
      background: `hex("f6f7f9"),
      surface: `hex("282c33c7"),
      accent: `hex("c6c6c6"),
      error: `hex("cf6679"),
    }
  };

let defaultTheme = Themes.Dark;
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