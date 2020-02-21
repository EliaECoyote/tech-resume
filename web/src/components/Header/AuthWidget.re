module Styles = {
  open Css;

  let loggedUserGrid =
    style([
      display(`grid),
      height(`rem(3.0)),
      gridTemplateRows([`percent(50.0), `percent(50.0)]),
      gridTemplateColumns([`fr(1.0), `rem(3.0)]),
      gridTemplateAreas(`areas(["username avatar", "cta avatar"])),
      gridColumnGap(`rem(1.0)),
    ]);

  let username =
    [
      gridArea(`ident("username")),
      textOverflow(`ellipsis),
      whiteSpace(`nowrap),
      overflow(`hidden),
    ]
    |> (styles => List.concat([styles, TextStyles.textEllipsis]))
    |> (styles => List.concat([styles, TextStyles.xsFont]))
    |> style;

  let cta = style([gridArea(`ident("cta"))]);

  let thumb =
    style([
      gridArea(`ident("avatar")),
      width(`percent(100.0)),
      height(`percent(100.0)),
      objectFit(`cover),
      borderRadius(`px(8)),
    ]);
};

let firebaseUiElementId = "auth";

[@react.component]
let make = (~authStatus: UseAuth.status, ~signOut: unit => unit) => {
  let firebaseUiService =
    UseLazyRef.hook(() => Service.FirebaseUi.make(firebaseUiElementId));

  React.useEffect2(
    () => {
      switch (authStatus) {
      | UseAuth.Anonymous =>
        firebaseUiService.startFirebaseUi() |> Belt2.Wonka.getEffectCleanup
      | _ => None
      }
    },
    (authStatus, firebaseUiService),
  );

  switch (authStatus) {
  | UseAuth.Idle => ReasonReact.null
  | UseAuth.Logged(user) =>
    <div className=Styles.loggedUserGrid>
      <span className=Styles.username>
        {Belt.Option.getWithDefault(user.displayName, "Unknown user")
         |> React.string}
      </span>
      <Button
        className=Styles.cta size=Controls.Small onClick={_ => signOut()}>
        {React.string("Log out")}
      </Button>
      // user mini thumb rendering
      {switch (user.photoURL) {
       | Some(url) => <img src=url className=Styles.thumb />
       | None => ReasonReact.null
       }}
    </div>
  | UseAuth.Anonymous => <div id=firebaseUiElementId />
  };
};