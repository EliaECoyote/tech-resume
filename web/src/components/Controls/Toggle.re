module Styles = {
  open Css;
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

  let thumb = (colors: ThemeContext.colors, ~checked: bool, ~hasFocus: bool) =>
    style([
      position(`absolute),
      top(`px(1)),
      bottom(`px(1)),
      left(`px(1)),
      width(`px(22)),
      borderRadius(`percent(50.0)),
      backgroundColor(`hex("fafafa")),
      transition(~duration=300, ~timingFunction=`easeInOut, "all"),
      transform(translateX(`px(checked ? 0 : 26))),
      hasFocus
        ? boxShadow(
            Shadow.box(
              ~x=`zero,
              ~y=`zero,
              ~blur=`px(5),
              ~spread=`px(5),
              colors.primary,
            ),
          )
        : active([
            boxShadow(
              Shadow.box(
                ~x=`zero,
                ~y=`zero,
                ~blur=`px(2),
                ~spread=`px(3),
                colors.primary,
              ),
            ),
          ]),
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
  let trackRef = React.useRef(Js.Nullable.null);
  let (theme, _) = React.useContext(ThemeContext.context);

  let (hasFocus, setHasFocus) = React.useState(() => false);

  React.useEffect0(() => {
    let windowElement = DomHelpers.windowElement(Webapi.Dom.window);
    let trackElement = React.Ref.current(trackRef) |> Js.Nullable.toOption;
    trackElement
    |> Belt.Option.flatMap(_, element =>
         Wonka.merge([|
           Wonka.fromDomEvent(element, "mousedown"),
           Wonka.fromDomEvent(element, "touchstart"),
         |])
         |> Wonka.onPush((. event) => {
              Webapi.Dom.Event.preventDefault(event);
              setHasFocus(_prev => true);
            })
         |> Wonka.switchMap((. _event) =>
              Wonka.merge([|
                Wonka.fromDomEvent(windowElement, "mouseup"),
                Wonka.fromDomEvent(windowElement, "touchend"),
                Wonka.fromDomEvent(windowElement, "touchcancel"),
              |])
            )
         |> Wonka.onPush((. _event) => setHasFocus(_prev => false))
         |> Wonka.subscribe((. event) => {
              // Webapi.Dom.Event.preventDefault(event)
              ()
            })
         |> WonkaHelpers.getEffectCleanup
       );
  });

  <div
    onClick={_ => onChange(!checked)}
    className={Styles.track(theme.colors)}
    ref={ReactDOMRe.Ref.domRef(trackRef)}>
    {renderUnchecked()}
    {renderChecked()}
    <div className={Styles.thumb(theme.colors, ~checked, ~hasFocus)} />
  </div>;
};