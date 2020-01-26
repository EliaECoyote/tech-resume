module Styles = {
  open Css;
  let bar = style([flex3(~grow=0.0, ~shrink=0.0, ~basis=`px(20))]);
};

[@react.component]
let make = () => {
  let barRef = React.useRef(Js.Nullable.null);

  React.useEffect0(() => {
    let element = barRef |> React.Ref.current |> Js.Nullable.toOption;

    let touchStartSubscription =
      element
      |> Belt.Option.map(_, element =>
           Wonka.merge([|
             Wonka.fromDomEvent(element, "mousedown"),
             Wonka.fromDomEvent(element, "touchstart"),
           |])
         )
      |> Belt.Option.getWithDefault(_, Wonka.never)
      |> Wonka.subscribe((. event) => Js.log(event));

    Some(() => {touchStartSubscription.unsubscribe()});
  });

  <div className=Styles.bar ref={ReactDOMRe.Ref.domRef(barRef)} />;
};