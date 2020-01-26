module Styles = {
  open Css;
  let bar = style([flex3(~grow=0.0, ~shrink=0.0, ~basis=`px(20))]);
};

[@react.component]
let make = (~children) => {
  let barRef = React.useRef(Js.Nullable.null);

  React.useEffect0(() => {
    let mouseDownSubscription =
      barRef
      |> React.Ref.current
      |> Js.Nullable.toOption
      |> Belt.Option.map(_, element =>
           Wonka.fromDomEvent(element, "mousedown")
         )
      |> Belt.Option.getWithDefault(_, Wonka.never)
      |> Wonka.subscribe((. event) => Js.log(event));

    let touchStartSubscription =
      barRef
      |> React.Ref.current
      |> Js.Nullable.toOption
      |> Belt.Option.map(_, element =>
           Wonka.fromDomEvent(element, "touchstart")
         )
      |> Belt.Option.getWithDefault(_, Wonka.never)
      |> Wonka.subscribe((. event) => Js.log(event));

    Some(
      () => {
        mouseDownSubscription.unsubscribe();
        touchStartSubscription.unsubscribe();
      },
    );
  });

  <div className=Styles.bar ref={ReactDOMRe.Ref.domRef(barRef)}>
    children
  </div>;
};