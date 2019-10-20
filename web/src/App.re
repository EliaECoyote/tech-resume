module AppContent = {
  [@react.component]
  let make = (~children) => {
    let (state, _) = React.useContext(ThemeContext.context);
    let bgColorString = Css.Types.Color.toString(state.colors.background);
    let globalStyle = {j|
      body {
        background-color: $bgColorString;
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