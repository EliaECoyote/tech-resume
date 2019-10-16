[@react.component]
let make = () => {
  let editorRef = React.useRef(Js.Nullable.null);

  React.useEffect0(() => {
    let editor = React.Ref.current(editorRef);
    let monacoOptions = Monaco.monacoOptions(~value="", ~language="markdown");
    switch (Js.Nullable.toOption(editor)) { 
    | Some(value) => Monaco.startupMonaco(value, monacoOptions)
    | None => Js.log("Monaco could not startup properly")
    };
    None;
  });

  <div ref={ref => React.Ref.setCurrent(editorRef, ref)}>
    {React.string("Hello!")}
  </div>;
};

let default = make;