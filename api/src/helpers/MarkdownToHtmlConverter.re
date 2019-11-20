// Pandoc (https://pandoc.org/) service wrapper

type template =
  | Standard;

let getTemplatePath = template =>
  switch (template) {
  | Standard => "styles/teemplate1.tex"
  };

let transform = (src: string): Js.Promise.t(ChildProcess.spawnResult) =>
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
      RemarkDocument.options(
        ~title="test title",
        ~style=[|"h1 { color: green }"|],
        (),
      );
    Unified.unified()
    ->Unified.use(~plugin=RemarkParse.markdown, ())
    ->Unified.use(~plugin=RemarkRehype.remark2rehype, ())
    ->Unified.use(~plugin=RemarkDocument.doc, ~options=remarkOpt, ())
    ->Unified.use(~plugin=RemarkDocument.doc, ())
    ->Unified.use(~plugin=RehypeFormat.format, ())
    ->Unified.use(~plugin=RehypeStringify.html, ())
    ->Unified.process(src, processCallback);
  });

let run =
    (~src: string, ~template: template)
    : Js.Promise.t(ChildProcess.spawnResult) => {
  // let templateFile = getTemplatePath(template);
  transform(src);
};