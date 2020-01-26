module Styles = {
  open Css;
  let bar =
    style([
      flex3(~grow=0.0, ~shrink=0.0, ~basis=`px(20)),
      cursor(`colResize),
    ]);
};

[@react.component]
let make = () => {
  let barRef = React.useRef(Js.Nullable.null);

  React.useEffect0(() => {
    barRef
    |> React.Ref.current
    |> Js.Nullable.toOption
    |> Belt.Option.map(_, element =>
         Wonka.merge([|
           Wonka.fromDomEvent(element, "mousedown"),
           Wonka.fromDomEvent(element, "touchstart"),
         |])
       )
    |> Belt.Option.getWithDefault(_, Wonka.never)
    |> Wonka.switchMap((. _) =>
         Webapi.Dom.window
         |> DomHelpers.windowElement
         |> (
           element =>
             Wonka.merge([|
               Wonka.fromDomEvent(element, "mousemove"),
               Wonka.fromDomEvent(element, "touchmove"),
             |])
         )
         |> Wonka.takeUntil(
              Webapi.Dom.window
              |> DomHelpers.windowElement
              |> (
                element =>
                  Wonka.merge([|
                    Wonka.fromDomEvent(element, "mouseup"),
                    Wonka.fromDomEvent(element, "touchend"),
                    Wonka.fromDomEvent(element, "touchcancel"),
                  |])
              ),
            )
       )
    |> Wonka.map((. event) => DomHelpers.asMouseEvent(event))
    |> Wonka.map((. event) => Webapi.Dom.MouseEvent.clientX(event))
    |> Wonka.tap((. x) => Js.log(("touchstart", x)))
    |> Wonka.subscribe((. event) => ())
    |> WonkaHelpers.getEffectCleanup
  });

  // React.useEffect0(() => {
  //   barRef
  //   |> React.Ref.current
  //   |> Js.Nullable.toOption
  //   |> Belt.Option.map(_, element =>
  //        Wonka.merge([|
  //          Wonka.fromDomEvent(element, "mousemove"),
  //          Wonka.fromDomEvent(element, "touchmove"),
  //        |])
  //      )
  //   |> Belt.Option.getWithDefault(_, Wonka.never)
  //   |> Wonka.map((. event) => DomHelpers.asMouseEvent(event))
  //   |> Wonka.map((. event) => Webapi.Dom.MouseEvent.clientX(event))
  //   |> Wonka.tap((. x) => Js.log(("touchMove", x)))
  //   |> Wonka.subscribe((. coordinates) => ())
  //   |> WonkaHelpers.getEffectCleanup
  // });

  <div className=Styles.bar ref={ReactDOMRe.Ref.domRef(barRef)} />;
};