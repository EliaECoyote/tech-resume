module Styles = {
  open Css;
  let verticallyCenteredStyles = [display(`flex), alignItems(`center)];

  let header =
    style([
      display(`grid),
      height(`percent(100.0)),
      gridColumnGap(`px(20)),
      padding2(~v=`px(0), ~h=`px(20)),
      gridTemplateColumns([`auto, `fr(1.0), `auto, `rem(16.0)]),
      gridTemplateAreas(`areas(["title spacer theme-selector auth-widget"])),
      alignItems(`center),
    ]);
  let title = style([gridArea(`ident("title")), margin(`zero)]);
  let themeSelector =
    style([
      gridArea(`ident("theme-selector")),
      display(`flex),
      height(`percent(100.0)),
      alignItems(`flexEnd),
    ]);
  let spacer = style([gridArea(`ident("title"))]);
  let authWidget =
    style([gridArea(`ident("auth-widget")), justifyContent(`flexEnd)]);
};

[@react.component]
let make = (~children, ~className=?) => {
  let className =
    className
    |> Belt.Option.map(_, value => Css.merge([Styles.header, value]))
    |> Belt.Option.getWithDefault(_, Styles.header);
  <header className>
    <h1 className=Styles.title> {React.string("# Tech resume")} </h1>
    <div className=Styles.themeSelector> <ToggleTheme /> </div>
    <div className=Styles.authWidget> children </div>
  </header>;
};