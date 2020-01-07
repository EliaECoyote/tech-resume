module Styles = {
  open Css;
  let verticallyCenteredStyles = [display(`flex), alignItems(`center)];

  let header =
    style([
      display(`grid),
      height(`px(80)),
      gridColumnGap(`px(20)),
      padding2(~v=`px(0), ~h=`px(20)),
      gridTemplateColumns([`auto, `fr(1.0), `auto, `rem(14.0)]),
      gridTemplateAreas(`areas(["title spacer theme-selector auth-widget"])),
      alignItems(`center),
    ]);
  let title = style([gridArea(`ident("title"))]);
  let themeSelector =
    style([
      gridArea(`ident("theme-selector")),
      flexDirection(`column),
      justifyContent(`center),
      display(`grid),
      gridTemplateRows([`fr(1.0), `auto]),
      height(`rem(3.0)),
      ...TextStyles.xsFont,
    ]);
  let spacer = style([gridArea(`ident("title"))]);
  let authWidget =
    style([gridArea(`ident("auth-widget")), justifyContent(`flexEnd)]);
};

[@react.component]
let make = (~children) =>
  <header className=Styles.header>
    <h1 className=Styles.title> {React.string("# Tech resume")} </h1>
    <div className=Styles.themeSelector>
      <span> {React.string("Theme")} </span>
      <ThemeDropdown />
    </div>
    <div className=Styles.authWidget> children </div>
  </header>;