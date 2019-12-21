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
let make = () =>
  <header className=Styles.header>
    <h1 className=Styles.headerTitle> {React.string("Tech resume")} </h1>
    <ThemeDropdown />
    <div id="auth" />
  </header>;