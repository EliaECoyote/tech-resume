// Pandoc (https://pandoc.org/) service wrapper

type template =
  | Standard;

let getTemplatePath = template =>
  switch (template) {
  | Standard => "styles/teemplate1.tex"
  };

let run =
    (~src: string, ~template: template)
    : Js.Promise.t(ChildProcess.spawnResult) => {
  let path = "scripts/generatePdf.sh";
  // let path = "sh scripts/generatePdf.sh";
  let templateFile = getTemplatePath(template);
  let args = [|
    // "--variable",
    // "papersize=A4",
    // "--verbose",
    // "--template=" ++ templateFile,
    // "--pdf-engine=pdflatex",
    // "-t",
    // "latex",
    // "--standalone",
    // "--output=pandoc_stdout.pdf",
  |];
  Js.log(("spawning process", "pandoc", args, src));
  ChildProcess.spawn(~path, ~args, ~src);
};