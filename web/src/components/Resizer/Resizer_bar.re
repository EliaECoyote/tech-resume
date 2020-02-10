module Styles = {
  open Css;
  let bar =
    style([
      flex3(~grow=0.0, ~shrink=0.0, ~basis=`px(12)),
      cursor(`colResize),
      display(`flex),
      opacity(0.8),
      flexDirection(`column),
      justifyContent(`center),
      alignItems(`center),
      hover([opacity(1.0)]),
      backgroundColor(`hex("000")),
      transition(~duration=200, "opacity"),
    ]);

  let dragIndicator = (colors: ThemeContext.colors) =>
    style([
      width(`px(4)),
      height(`percent(30.0)),
      backgroundColor(colors.accent),
      borderRadius(`px(50)),
    ]);
};

[@react.component]
let make = (~onResizeEnd=() => ()) => {
  let barRef = React.useRef(Js.Nullable.null);
  let (theme, _) = React.useContext(ThemeContext.context);
  let (state, dispatch) = React.useContext(Resizer_context.context);

  // setting context state dimensions in react refs in order to
  // avoid re-applying useEffect listeners upon value update
  let offsetRef = React.useRef(state.resizerOffset);
  let widthRef = React.useRef(state.resizerWidth);
  offsetRef |> React.Ref.setCurrent(_, state.resizerOffset);
  widthRef |> React.Ref.setCurrent(_, state.resizerWidth);

  React.useEffect0(() => {
    let windowElement = DomHelpers.windowElement(Webapi.Dom.window);
    let barElement = React.Ref.current(barRef) |> Js.Nullable.toOption;
    barElement
    |> Belt.Option.map(_, element =>
         Wonka.merge([|
           Wonka.fromDomEvent(element, "mousedown"),
           Wonka.fromDomEvent(element, "touchstart"),
         |])
       )
    |> Belt.Option.getWithDefault(_, Wonka.never)
    |> Wonka.onPush((. event) => {
         Webapi.Dom.Event.preventDefault(event);
         dispatch(Resizer_context.StartResizing);
       })
    |> Wonka.switchMap((. _) =>
         Wonka.merge([|
           Wonka.fromDomEvent(windowElement, "mousemove"),
           Wonka.fromDomEvent(windowElement, "touchmove"),
         |])
         |> Wonka.takeUntil(
              Wonka.merge([|
                Wonka.fromDomEvent(windowElement, "mouseup"),
                Wonka.fromDomEvent(windowElement, "touchend"),
                Wonka.fromDomEvent(windowElement, "touchcancel"),
              |])
              |> Wonka.take(1)
              |> Wonka.onPush((. _event) => {
                   onResizeEnd();
                   dispatch(Resizer_context.EndResizing);
                 }),
            )
       )
    |> Wonka.map((. event) =>
         DomHelpers.asMouseEvent(event)
         |> Webapi.Dom.MouseEvent.clientX
         |> Js.Int.toFloat
       )
    |> Wonka.subscribe((. x) => {
         let (width, offset) = (
           React.Ref.current(widthRef),
           React.Ref.current(offsetRef),
         );
         // represents the cursor position based on the resizer
         // width, without considering the resizer offsetLeft
         let cursorResizerBasedPosition = x -. offset;
         // represents the cursor position in a 1.0 based proportion
         let cursorPositionInResizerProportion =
           cursorResizerBasedPosition /. width;
         // multiplies the cursor position in proportion by the
         // maximum flex-grow value (2.0)
         let leftFlexGrowValue = cursorPositionInResizerProportion *. 2.0;
         // by substracting the left section flex-grow value from
         // the maximum flex-grow value, you obtain the right section
         // flex-grow value
         let rightFlexGrowValue = 2.0 -. leftFlexGrowValue;
         let sizes = (leftFlexGrowValue, rightFlexGrowValue);
         dispatch(Resizer_context.UpdateSectionsFlexGrow(sizes));
         ();
       })
    |> WonkaHelpers.getEffectCleanup;
  });

  <div className=Styles.bar ref={ReactDOMRe.Ref.domRef(barRef)}>
    <div className={Styles.dragIndicator(theme.colors)} />
  </div>;
};