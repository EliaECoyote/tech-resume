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
  let hasFocusRef = React.useRef(hasFocus);
  React.Ref.setCurrent(hasFocusRef, hasFocus);

  React.useEffect0(() => {
    let windowElement = DomHelpers.windowElement(Webapi.Dom.window);
    React.Ref.current(trackRef)
    |> Js.Nullable.toOption
    |> Belt.Option.flatMap(_, element =>
         Wonka.merge([|
           Wonka.fromDomEvent(element, "mousedown"),
           Wonka.fromDomEvent(element, "touchstart"),
         |])
         |> Wonka.onPush((. event) =>
              Webapi.Dom.Event.preventDefault(event)
            )
         |> Wonka.map((. _) => React.Ref.current(hasFocusRef))
         |> Wonka.onPush((. _) => setHasFocus(_prev => true))
         |> Wonka.switchMap((. hadFocusOnTouchStart) =>
              Wonka.merge([|
                Wonka.fromDomEvent(windowElement, "mouseup"),
                Wonka.fromDomEvent(windowElement, "touchend"),
                Wonka.fromDomEvent(windowElement, "touchcancel"),
              |])
              |> Wonka.take(1)
              |> Wonka.map((. _) => hadFocusOnTouchStart)
            )
         |> Wonka.subscribe((. hadFocusOnTouchStart) => {
              if (!hadFocusOnTouchStart) {
                setHasFocus(_prev => false);
              };
              let inputElement =
                React.Ref.current(inputRef) |> Js.Nullable.toOption;
              switch (inputElement) {
              | Some(element) =>
                let _ =
                  element
                  |> Webapi.Dom.HtmlElement.ofElement
                  |> Belt.Option.map(_, Webapi.Dom.HtmlElement.focus);
                ();
              | None => ()
              };
            })
         |> WonkaHelpers.getEffectCleanup
       );
  });

  React.useEffect0(() => {
    React.Ref.current(inputRef)
    |> Js.Nullable.toOption
    |> Belt.Option.flatMap(_, element =>
         Wonka.fromDomEvent(element, "focus")
         |> Wonka.onPush((. _event) => setHasFocus(_prev => true))
         |> Wonka.switchMap((. _event) =>
              Wonka.fromDomEvent(element, "blur") |> Wonka.take(1)
            )
         |> Wonka.subscribe((. _event) => {setHasFocus(_prev => false)})
         |> WonkaHelpers.getEffectCleanup
       )
  });

  <div
    onClick={_ => onChange(!checked)}
    className={Styles.track(theme.colors)}
    ref={ReactDOMRe.Ref.domRef(trackRef)}>
    {renderUnchecked()}
    {renderChecked()}
    <div className={Styles.thumb(theme.colors, ~checked, ~hasFocus)} />
    <input
      type_="checkbox"
      ariaLabel="Switch between Dark and Light mode"
      checked
      className=Styles.hiddenInput
      ref={ReactDOMRe.Ref.domRef(inputRef)}
    />
  </div>;
};