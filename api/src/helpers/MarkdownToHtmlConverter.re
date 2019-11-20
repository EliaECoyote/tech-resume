// Pandoc (https://pandoc.org/) service wrapper

type template =
  | Standard;

let getTemplateFontFamily = template =>
  switch (template) {
  | Standard => "'Roboto', sans-serif"
  };

let getTemplateCssResources = template =>
  switch (template) {
  | Standard => [|
      "https://fonts.googleapis.com/css?family=Roboto&display=swap",
    |]
  };

let getTemplateStyle = template => {
  let fontFamily = getTemplateFontFamily(template);
  switch (template) {
  | Standard => [|
      {j|
        body {
          background: white;
          font-family: $fontFamily;
        }
        h1 {
          color: green
        }
      |j},
    |]
  };
};

let transform =
    (~src: string, ~style: array(string), ~css: array(string))
    : Js.Promise.t(ChildProcess.spawnResult) =>
  Js.Promise.make((~resolve, ~reject as _) => {
    let resolve = (output: ChildProcess.spawnResult) => resolve(. output);
    let processCallback: Unified.Types.processCallback =
      (error, data) => {
        switch (Js.Nullable.toOption(error)) {
        | Some(value) =>
          Js.log(value);
          Js.Exn.message(value)
          ->Belt.Option.getWithDefault("generic converter error")
          ->ChildProcess.Error
          ->resolve;
        | None => VFile.contentsGet(data)->ChildProcess.Ok->resolve
        };
      };
    let remarkOpt =
      RemarkDocument.options(~title="test title", ~style, ~css, ());
    Unified.unified()
    ->Unified.use(~plugin=RemarkParse.markdown, ())
    ->Unified.use(~plugin=RemarkRehype.remark2rehype, ())
    ->Unified.use(~plugin=RemarkDocument.doc, ~options=remarkOpt, ())
    ->Unified.use(~plugin=RehypeFormat.format, ())
    ->Unified.use(~plugin=RehypeStringify.html, ())
    ->Unified.process(src, processCallback);
  });

let run =
    (~src: string, ~template: template)
    : Js.Promise.t(ChildProcess.spawnResult) => {
  let style = getTemplateStyle(template);
  let css = getTemplateCssResources(template);
  transform(~src, ~style, ~css);
};