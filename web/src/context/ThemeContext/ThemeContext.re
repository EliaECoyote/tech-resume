type colors = {
  primary: Css.Types.Color.t,
  secondary: Css.Types.Color.t,
  background: Css.Types.Color.t,
  surface: Css.Types.Color.t,
  accent: Css.Types.Color.t,
  error: Css.Types.Color.t,
  skeletonAccentColor: Css.Types.Color.t,
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
      primary: `hex("03dac6"),
      secondary: `hex("000"),
      background: `hex("282c33"),
      surface: `hex("282c33c7"),
      accent: `hex("03dac6"),
      error: `hex("cf6679"),
      skeletonAccentColor: `hex("1B1B1E"),
    }
  | Themes.Standard => {
      primary: `hex("4E8098"),
      secondary: `hex("fff"),
      background: `hex("FCF7F8"),
      surface: `hex("CED3DC"),
      accent: `hex("fff"),
      error: `hex("cf6679"),
      skeletonAccentColor: `hex("fff"),
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