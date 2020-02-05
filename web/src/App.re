module Styles = {
  open Css;
  // TODO: check if transition is actually applied
  // only after js evaluation! This is important in
  // order to avoid triggering the color transition
  // on page load
  global(
    "body",
    [
      transition(~duration=300, "color"),
      transition(~duration=300, "background-color"),
    ],
  );
  let app =
    style([
      display(`grid),
      height(`vh(100.0)),
      padding(`px(8)),
      gridTemplateRows([`px(50), `fr(1.0)]),
      gridTemplateAreas(`areas(["header", "content"])),
      gridRowGap(`px(10)),
    ]);
  let header = style([gridArea(`ident("header"))]);
  let content = style([gridArea(`ident("content"))]);
};

[%bs.raw {| require("firebase/app") |}];
[%bs.raw {| require("firebase/auth") |}];
[%bs.raw {| require("firebase/firestore") |}];

InitializeFirebase.init();

module AppContent = {
  [@react.component]
  let make = (~children) => {
    let (state, _) = React.useContext(ThemeContext.context);
    let (authStatus, signOut) = React.useContext(AuthContext.context);
    let background = Css.Types.Color.toString(state.colors.background);
    let primary = Css.Types.Color.toString(state.colors.primary);
    let globalStyle = {j|
      body {
        margin: 0;
        color: $primary;
        background-color: $background;
        font-family: 'Montserrat', sans-serif;
      }
      * { box-sizing: border-box; }
    |j};

    <div className=Styles.app>
      <ReactHelmet>
        <link
          href="https://fonts.googleapis.com/css?family=Montserrat&display=swap"
          rel="stylesheet"
        />
        <link
          type_="text/css"
          rel="stylesheet"
          href="https://www.gstatic.com/firebasejs/ui/4.3.0/firebase-ui-auth.css"
        />
        <style> {ReasonReact.string(globalStyle)} </style>
      </ReactHelmet>
      <Header className=Styles.header>
        <AuthWidget authStatus signOut />
      </Header>
      <div className=Styles.content> children </div>
    </div>;
  };
};

[@react.component]
let make = (~children) =>
  <AuthContext>
    <ThemeContext> <AppContent> children </AppContent> </ThemeContext>
  </AuthContext>;

let default = make;