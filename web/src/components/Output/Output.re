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
let make = () => {
  let iframeElementRef = React.useRef(Js.Nullable.null);
  let (content, setContent) =
    React.useState(() => "<div>loading data....</div>");

  React.useEffect0(() => {
    let _ =
      Fetch.fetch("http://localhost:3000")
      |> Js.Promise.then_(Fetch.Response.text)
      |> Js.Promise.then_(value => {
           Js.log(value);
           setContent(_ => value);
           Js.Promise.resolve();
         })
      |> Js.Promise.catch(error => {
           Js.log(error);
           Js.Promise.resolve();
         });
    None;
  });

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
    <button onClick> {React.string("download")} </button>
  </div>;
};

let default = make;