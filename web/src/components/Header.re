module Styles = {
  open Css;
  let header =
    style([
      display(`flex),
      justifyContent(`spaceBetween),
      alignItems(`center),
    ]);
  let headerTitle = style([]);
};

[@react.component]
let make = () => {
  let (state, _) = React.useContext(ThemeContext.context);
  <header className=Styles.header>
    <h1 className=Styles.headerTitle> {React.string("Markdown 2 cv")} </h1>
    <ThemeDropdown />
  </header>;
};