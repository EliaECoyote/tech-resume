module Styles = {
  open Css;

  let app =
    style([
      display(`grid),
      gridTemplateColumns([`percent(50.0), `percent(50.0)]),
      gridColumnGap(`pt(10)),
    ]);
};

[@react.component]
let make = (~content: string=?) => {
  let iframeElementRef = React.useRef(Js.Nullable.null);

  let onClick = _ => {
    let iframeElement =
      React.Ref.current(iframeElementRef)->Js.Nullable.toOption;
    switch (iframeElement) {
    | Some(element) => IFrame.getContentWindow(element)->IFrame.print
    | None => ()
    };
  };

  <div>
    <iframe srcDoc=content ref={ReactDOMRe.Ref.domRef(iframeElementRef)} />
    <Button onClick> {React.string("download")} </Button>
  </div>;
};

let default = make;