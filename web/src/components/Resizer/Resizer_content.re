module Styles = {
  open Css;
  let resizer = style([display(`flex)]);
};

[@react.component]
let make = (~children, ~className=?, ()) => {
  let resizerRef: React.Ref.t(Js.nullable(Dom.element)) =
    React.useRef(Js.Nullable.null);
  let (state, dispatch) = React.useContext(Resizer_context.context);

  // setting context state dimensions in react refs in order to
  // avoid re-applying useEffect listeners upon value update
  let previousOffset = React.useRef(state.resizerOffset);
  let previousWidth = React.useRef(state.resizerWidth);
  previousOffset |> React.Ref.setCurrent(_, state.resizerOffset);
  previousWidth |> React.Ref.setCurrent(_, state.resizerWidth);

  React.useEffect0(() => {
    let updateContextDimensions = () => {
      let element = React.Ref.current(resizerRef) |> Js.Nullable.toOption;
      switch (element) {
      | Some(element) =>
        let element = Webapi.Dom.Element.unsafeAsHtmlElement(element);
        let (offsetLeft, clientWidth) = (
          Webapi.Dom.HtmlElement.offsetLeft(element) |> Js.Int.toFloat,
          Webapi.Dom.HtmlElement.clientWidth(element) |> Js.Int.toFloat,
        );
        if (clientWidth !== React.Ref.current(previousWidth)) {
          dispatch(Resizer_context.UpdateResizerWidth(clientWidth));
        };
        if (offsetLeft !== React.Ref.current(previousOffset)) {
          dispatch(Resizer_context.UpdateResizerOffset(offsetLeft));
        };
      | None => ()
      };
    };

    updateContextDimensions();

    Webapi.Dom.window
    |> DomHelpers.windowElement
    |> Wonka.fromDomEvent(_, "resize")
    |> Wonka.subscribe((. _event) => updateContextDimensions())
    |> WonkaHelpers.getEffectCleanup;
  });

  let className = CssHelpers.combine([Some(Styles.resizer), className]);

  <div className ref={ReactDOMRe.Ref.domRef(resizerRef)}> children </div>;
};