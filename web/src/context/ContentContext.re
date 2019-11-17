// type state = {content: string};
 // type actions =
 //   | ChangeTheme(Themes.t);
 // let getColors = theme =>
 //   switch (theme) {
 //   | Themes.Black => {
 //       primary: `hex("fff"),
 //       background: `hex("000"),
 //       accent: `hex("f38518"),
 //     }
 //   | Themes.Dark => {
 //       primary: `hex("d4d4d4"),
 //       background: `hex("1e1e1e"),
 //       accent: `hex("c6c6c6"),
 //     }
 //   | Themes.Standard => {
 //       primary: `hex("000"),
 //       background: `hex("fffffe"),
 //       accent: `hex("c6c6c6"),
 //     }
 //   };
 // let defaultTheme = Themes.Dark;
 // let defaultDispatcher: actions => unit = _ => ();
 // let initialState: state = {
 //   theme: defaultTheme,
 //   colors: getColors(defaultTheme),
 // };
 // let context = React.createContext((initialState, defaultDispatcher));
 // let reducer = (_, action) =>
 //   switch (action) {
 //   | ChangeTheme(value) => {theme: value, colors: getColors(value)}
 //   };
 // module Provider = {
 //   let makeProps = (~value, ~children, ()) => {
 //     "value": value,
 //     "children": children,
 //   };
 //   let make = React.Context.provider(context);
 // };
 // [@react.component]
 // let make = (~children) => {
 //   let (state, dispatch) = React.useReducer(reducer, initialState);
 //   <div> <Provider value=(state, dispatch)> children </Provider> </div>;
 /* }*/