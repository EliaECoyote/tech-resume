type result =
  | Ok(string)
  | Error(string);

type t = result;

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

/**
 * converts markdown to html code. The **css** parameter will
 * be embedded as a style tag on the *head* of the document
 */
let transform =
    (~src: string, ~style: array(string), ~css: array(string))
    : Js.Promise.t(result) =>
  Js.Promise.make((~resolve, ~reject as _) => {
    let resolve = (output: result) => resolve(. output);
    let processCallback: Unified.Types.processCallback =
      (error, data) => {
        switch (Js.Nullable.toOption(error)) {
        | Some(value) =>
          Js.log(value);
          Js.Exn.message(value)
          ->Belt.Option.getWithDefault("generic converter error")
          ->Error
          ->resolve;
        | None => VFile.contentsGet(data)->Ok->resolve
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

/**
 * runs the md 2 html converter. The **template** parameter specifies
 * which template will be used to style the html document
 */
let run = (~src: string, ~template: template): Js.Promise.t(result) => {
  let style = getTemplateStyle(template);
  let css = getTemplateCssResources(template);
  transform(~src, ~style, ~css);
};