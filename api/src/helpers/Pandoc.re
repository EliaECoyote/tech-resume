// Pandoc (https://pandoc.org/) service wrapper

type fileKind =
  | Markdown
  | Html
  | Pdf
  | Context
  | Latex;

type template =
  | Standard;

let getFileKindValue = kind =>
  switch (kind) {
  | Markdown => "markdown"
  | Html => "html"
  | Pdf => "pdf"
  | Latex => "latex"
  | Context => "context"
  };

let getTemplatePath = template =>
  switch (template) {
  | Standard => "styles/template1.tex"
  };

let run =
    (
      ~src: string,
      ~fromKind: fileKind,
      ~toKind: fileKind,
      ~template: template,
    )
    : Js.Promise.t(ChildProcess.spawnResult) => {
  let path = "scripts/generatePdf.sh";
  let fromValue = getFileKindValue(fromKind);
  let toValue = getFileKindValue(toKind);
  let templateFile = getTemplatePath(template);
  let args = [|
    // "--template",
    // templateFile,
    // "--variable",
    // "papersize=A4",
    // "--pdf-engine=pdflatex",
    // "--verbose",
    // "-t",
    // "context",
    // "-f",
    // "markdown",
    // "--standalone",
    "--output=pandoc_stdout.pdf",
  |];
  Js.log(("spawning process", "pandoc", args, src));
  ChildProcess.spawn(~path, ~args, ~src);
};