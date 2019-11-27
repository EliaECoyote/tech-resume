let hook = () => {
  let iframeElementRef = React.useRef(Js.Nullable.null);

  let triggerPrint = _ => {
    let iframeElement =
      React.Ref.current(iframeElementRef)->Js.Nullable.toOption;
    switch (iframeElement) {
    | Some(element) => IFrame.getContentWindow(element)->IFrame.print
    | None => ()
    };
  };

  (iframeElementRef, triggerPrint);
};