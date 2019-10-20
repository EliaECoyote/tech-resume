module Styles = {
  /* Open the Css module, so we can access the style properties below without prefixing them with Css. */
  open Css;

  let editor =
    style([
      height(px(200)),
      backgroundColor(blue),
      // display(flexBox),
      // flexDirection(column),
      // alignItems(stretch),
      // backgroundColor(white),
      // boxShadow(Shadow.box(~y=px(3), ~blur=px(5), rgba(0, 0, 0, 0.3))),
      /* You can add non-standard and other unsafe style declarations using the `unsafe` function, with strings as the two arguments. */
      // unsafe("-webkit-overflow-scrolling", "touch"),
      /* You can place all your theme styles in Theme.re and access as normal Reason module */
      // padding(Theme.basePadding)
    ]);
  // let title = style([
  //   fontSize(rem(1.5)),
  //   color(Theme.textColor),
  //   marginBottom(Theme.basePadding)
  // ]);
  // let actionButton = disabled =>
  //   style([
  //     background(disabled ? darkgray : white),
  //     color(black),
  //     border(px(1), solid, black),
  //     borderRadius(px(3)),
  //   ])
};

[@react.component]
let make = () => {
  let editorRef = React.useRef(Js.Nullable.null);
  let monacoInstanceRef = React.useRef(None);

  let (state, _) = React.useContext(ConfigContext.context);

  React.useEffect1(
    () => {
      Monaco.setTheme(state.theme);
      None;
    },
    [|state.theme|],
  );

  React.useEffect0(() => {
    let monacoOptions =
      Monaco.monacoOptions(
        ~value="",
        ~language="markdown",
        ~automaticLayout=true,
        ~minimap=Monaco.monacoMinimap(~enabled=false),
      );

    React.Ref.current(editorRef)
    ->Js.Nullable.toOption
    ->Belt.Option.map(value => Monaco.create(value, monacoOptions))
    |> React.Ref.setCurrent(monacoInstanceRef);

    let disposeOfMonacoInstance = () =>
      React.Ref.current(monacoInstanceRef)
      ->Belt.Option.map(value => Monaco.dispose(value))
      ->(_ => ());
    Some(disposeOfMonacoInstance);
  });

  <div>
    <div className=Styles.editor ref={ReactDOMRe.Ref.domRef(editorRef)} />
  </div>;
};