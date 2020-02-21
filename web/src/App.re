module Styles = {
  open Css;

  global("body", [margin(`zero), fontFamily("'Montserrat', sans-serif")]);
  global("*", [boxSizing(`borderBox)]);
  // Firebase ui custom styles
  global(
    ".firebaseui-container .firebaseui-idp-list, "
    ++ ".firebaseui-container .firebaseui-idp-list>.firebaseui-list-item, "
    ++ ".firebaseui-container .firebaseui-tenant-list>.firebaseui-list-item",
    [margin(`zero)],
  );

  let app = (colors: ThemeContext.colors) =>
    style([
      display(`grid),
      height(`vh(100.0)),
      padding(`px(8)),
      gridTemplateRows([`px(50), `fr(1.0)]),
      gridTemplateAreas(`areas(["header", "content"])),
      gridRowGap(`px(10)),
      color(colors.primary),
      backgroundColor(colors.background),
    ]);
  let header = style([gridArea(`ident("header"))]);
  let content = style([gridArea(`ident("content")), overflow(`hidden)]);
};

if (Config.isBrowser) {
  Belt2.Firebase.init();
};

module AppContent = {
  [@react.component]
  let make = (~children) => {
    let (themeState, _) = React.useContext(ThemeContext.context);
    let (authStatus, signOut) = React.useContext(AuthContext.context);

    <div className={Styles.app(themeState.colors)}>
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
      </ReactHelmet>
      <Header className=Styles.header>
        <AuthWidget authStatus signOut />
      </Header>
      <main className=Styles.content> children </main>
    </div>;
  };
};

[@react.component]
let make = (~children) =>
  <AuthContext>
    <ThemeContext> <AppContent> children </AppContent> </ThemeContext>
  </AuthContext>;

let default = make;