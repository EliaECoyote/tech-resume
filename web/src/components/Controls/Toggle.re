module Styles = {
  open Css;

  let highlighColor = `hex("03dac6");

  let track = (_colors: ThemeContext.colors) =>
    style([
      height(`px(24)),
      width(`px(50)),
      cursor(`pointer),
      display(`flex),
      justifyContent(`spaceBetween),
      borderRadius(`px(30)),
      backgroundColor(`hex("000")),
      padding2(~v=`px(2), ~h=`px(3)),
      position(`relative),
    ]);

  let bigShadowStyle =
    boxShadow(
      Shadow.box(
        ~x=`zero,
        ~y=`zero,
        ~blur=`px(5),
        ~spread=`px(5),
        highlighColor,
      ),
    );

  let smallShadowStyles =
    boxShadow(
      Shadow.box(
        ~x=`zero,
        ~y=`zero,
        ~blur=`px(2),
        ~spread=`px(3),
        highlighColor,
      ),
    );

  let thumb = (colors: ThemeContext.colors, ~checked: bool, ~hasFocus: bool) => {
    let addFocusStylesIfNeeded = rules =>
      hasFocus ? List.append(rules, [smallShadowStyles]) : rules;
    style(
      [
        position(`absolute),
        top(`px(1)),
        bottom(`px(1)),
        left(`px(1)),
        width(`px(22)),
        borderRadius(`percent(50.0)),
        backgroundColor(`hex("fafafa")),
        transition(~duration=300, ~timingFunction=`easeInOut, "all"),
        transform(translateX(`px(checked ? 0 : 26))),
        active([bigShadowStyle]),
      ]
      |> addFocusStylesIfNeeded,
    );
  };

  let hiddenInput =
    style([
      borderWidth(`zero),
      height(`px(1)),
      margin(`px(-1)),
      overflow(`hidden),
      padding(`zero),
      position(`absolute),
      width(`px(1)),
      unsafe("clip", "rect(0,0,0,0)"),
    ]);
};

[@react.component]
let make =
    (
      ~renderChecked: unit => React.element,
      ~renderUnchecked: unit => React.element,
      ~onChange: bool => unit,
      ~checked,
    ) => {
  let (theme, _) = React.useContext(ThemeContext.context);
  let trackRef = React.useRef(Js.Nullable.null);
  let inputRef = React.useRef(Js.Nullable.null);
  let (hasFocus, setHasFocus) = React.useState(() => false);

  React.useEffect0(() => {
    let windowElement = DomHelpers.windowElement(Webapi.Dom.window);
    React.Ref.current(trackRef)
    |> Js.Nullable.toOption
    |> Belt.Option.flatMap(_, element =>
         XWonka.merge([|
           XWonka.fromDomEvent(element, "mousedown"),
           XWonka.fromDomEvent(element, "touchstart"),
         |])
         |> XWonka.onPush(Webapi.Dom.Event.preventDefault)
         |> XWonka.switchMap(hadFocusOnTouchStart =>
              XWonka.merge([|
                XWonka.fromDomEvent(windowElement, "mouseup"),
                XWonka.fromDomEvent(windowElement, "touchend"),
                XWonka.fromDomEvent(windowElement, "touchcancel"),
              |])
              |> XWonka.take(1)
              |> XWonka.map(_ => hadFocusOnTouchStart)
            )
         |> XWonka.publish
         |> XWonka.getEffectCleanup
       );
  });

  React.useEffect0(() => {
    React.Ref.current(inputRef)
    |> Js.Nullable.toOption
    |> Belt.Option.flatMap(_, element =>
         XWonka.fromDomEvent(element, "focus")
         |> XWonka.onPush(_event => setHasFocus(_prev => true))
         |> XWonka.switchMap(_event =>
              XWonka.fromDomEvent(element, "blur") |> XWonka.take(1)
            )
         |> XWonka.subscribe(_event => {setHasFocus(_prev => false)})
         |> XWonka.getEffectCleanup
       )
  });

  <div
    onClick={_ => {
      let _ =
        React.Ref.current(inputRef)
        |> Js.Nullable.toOption
        |> Belt.Option.flatMap(_, Webapi.Dom.HtmlElement.ofElement)
        |> Belt.Option.map(
             _,
             el => {
               Webapi.Dom.HtmlElement.focus(el);
               Webapi.Dom.HtmlElement.click(el);
             },
           );
      ();
    }}
    className={Styles.track(theme.colors)}
    ref={ReactDOMRe.Ref.domRef(trackRef)}>
    {renderUnchecked()}
    {renderChecked()}
    <div className={Styles.thumb(theme.colors, ~checked, ~hasFocus)} />
    <input
      type_="checkbox"
      ariaLabel="Switch between Dark and Light mode"
      onChange={event => {
        let target = ReactEvent.Form.target(event);
        let checked = target##checked;
        onChange(checked);
        ();
      }}
      checked
      className=Styles.hiddenInput
      ref={ReactDOMRe.Ref.domRef(inputRef)}
    />
  </div>;
};