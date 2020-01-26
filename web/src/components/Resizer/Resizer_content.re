module Styles = {
  open Css;
  let resizer = style([display(`flex)]);
};

[@react.component]
let make = (~children) => {
  let resizerRef = React.useRef(Js.Nullable.null);

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

  <div className=Styles.resizer ref={ReactDOMRe.Ref.domRef(resizerRef)}>
    children
  </div>;
};

let container = Resizer_container.make;