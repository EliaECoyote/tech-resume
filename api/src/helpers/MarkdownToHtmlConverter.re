// Pandoc (https://pandoc.org/) service wrapper

type template =
  | Standard;

let getTemplatePath = template =>
  switch (template) {
  | Standard => "styles/teemplate1.tex"
  };

let transform: string => Js.Promise.t(string) = [%bs.raw
  {|
      (src) => {
        var vfile = require('to-vfile')
        var unified = require('unified')
        var markdown = require('remark-parse')
        var remark2rehype = require('remark-rehype')
        var doc = require('rehype-document')
        var format = require('rehype-format')
        var html = require('rehype-stringify')

        return new Promise((resolve, reject) => {
          unified()
            .use(markdown)
            .use(remark2rehype)
            .use(doc)
            .use(format)
            .use(html)
            .process('# Test *bold text*', (err, file) => {
              if (err) {
                console.error(err);
                reject(err);
              } else {
                resolve(file.contents);
              }
            })
        });
      }
  |}
];

let run =
    (~src: string, ~template: template)
    : Js.Promise.t(ChildProcess.spawnResult) => {
  let templateFile = getTemplatePath(template);
  transform("test")
  |> Js.Promise.then_(result => ChildProcess.Ok(result)->Js.Promise.resolve)
  |> Js.Promise.catch(_ =>
       "Generic error"->ChildProcess.Error->Js.Promise.resolve
     );
};