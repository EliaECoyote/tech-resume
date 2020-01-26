module Styles = {
  open Css;
  let resizer = style([display(`flex)]);
};

[@react.component]
let make = (~children, ~className=?, ()) => {
  let resizerRef = React.useRef(Js.Nullable.null);
  let (state, dispatch) = React.useContext(Resizer_context.context);

  React.useEffect0(() => {
    Webapi.Dom.document
    |> Webapi.Dom.Document.documentElement
    |> Wonka.fromDomEvent(_, "resize")
    |> Wonka.subscribe((. event) =>
         Js.Console.warn((
           "resize event: ",
           event,
           "resizerRef: ",
           resizerRef,
         ))
       )
    |> WonkaHelpers.getEffectCleanup
  });

  let className = CssHelpers.combine([Some(Styles.resizer), className]);
  <div className ref={ReactDOMRe.Ref.domRef(resizerRef)}> children </div>;
};

let container = Resizer_container.make;