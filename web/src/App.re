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
      }
      * { box-sizing: border-box; }
    |j};

    <div>
      <ReactHelmet>
        <link
          href="https://fonts.googleapis.com/css?family=Montserrat&display=swap"
          rel="stylesheet"
        />
        <style> {ReasonReact.string(globalStyle)} </style>
      </ReactHelmet>
      <Header />
      children
    </div>;
  };
};

[@react.component]
let make = (~children) =>
  <ThemeContext> <AppContent> children </AppContent> </ThemeContext>;