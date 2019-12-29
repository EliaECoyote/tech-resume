module AppContent = {
  [@react.component]
  let make = (~children) => {
    let (state, _) = React.useContext(ThemeContext.context);
    let background = Css.Types.Color.toString(state.colors.background);
    let primary = Css.Types.Color.toString(state.colors.primary);
    let globalStyle = {j|
      body {
        color: $primary;
        background-color: $background;
        font-family: 'Montserrat', sans-serif;
        transition: color, background-color 0.3s
      }
      * { box-sizing: border-box; }
    |j};

    UseFirebase.hook();
    let (authStatus, signOut) = UseAuth.hook();

    <div>
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
      <Header> <AuthWidget authStatus signOut /> </Header>
      children
    </div>;
  };
};

[@react.component]
let make = (~children) =>
  <ThemeContext> <AppContent> children </AppContent> </ThemeContext>;

let default = make;