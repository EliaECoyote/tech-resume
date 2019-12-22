module Styles = {
  open Css;
  let header =
    style([
      display(`flex),
      padding2(~v=`px(0), ~h=`px(20)),
      justifyContent(`spaceBetween),
      alignItems(`center),
    ]);
  let headerTitle = style([]);
};

[@react.component]
let make = (~children) =>
  <header className=Styles.header>
    <div>
      <h1 className=Styles.headerTitle> {React.string("Tech resume")} </h1>
      <ThemeDropdown />
    </div>
    <div> children </div>
  </header>;