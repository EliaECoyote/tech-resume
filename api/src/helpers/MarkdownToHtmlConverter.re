// Pandoc (https://pandoc.org/) service wrapper

type template =
  | Standard;

let getTemplatePath = template =>
  switch (template) {
  | Standard => "styles/teemplate1.tex"
  };

let transform = (src: string): Js.Promise.t(ChildProcess.spawnResult) =>
  Js.Promise.make((~resolve, ~reject) => {
    let processCallback: Unified.Types.processCallback =
      (error, data) => {
        switch (Js.Nullable.toOption(error)) {
        | Some(value) =>
          Js.Exn.message(value)
          ->Belt.Option.getWithDefault("generic md2html error")
          ->ChildProcess.Error
          ->(output => resolve(. output))
        | None =>
          switch (data) {
          | `Str(value) => resolve(. ChildProcess.Ok(value))
          | `Buf(value) =>
            Buffer.contents(value)
            ->ChildProcess.Ok
            ->(output => resolve(. output))
          }
        };
      };
    Unified.unified()
    ->Unified.use(RemarkParse.markdown)
    ->Unified.use(RemarkRehype.remark2rehype)
    ->Unified.use(RemarkDocument.doc)
    ->Unified.use(RehypeFormat.format)
    ->Unified.use(RehypeStringify.html)
    ->Unified.process(src, processCallback);
  });

// [%bs.raw
//   {|
//       (src) => {
//         var unified = require('unified')
//         var markdown = require('remark-parse')
//         var remark2rehype = require('remark-rehype')
//         var doc = require('rehype-document')
//         var format = require('rehype-format')
//         var html = require('rehype-stringify')

//         return new Promise((resolve, reject) => {
//           unified()
//             .use(markdown)
//             .use(remark2rehype)
//             .use(doc)
//             .use(format)
//             .use(html)
//             .process('# Test *bold text*', (err, file) => {
//               if (err) {
//                 console.error(err);
//                 reject(err);
//               } else {
//                 resolve(file.contents);
//               }
//             })
//         });
//       }
//   |}
// ];

let run =
    (~src: string, ~template: template)
    : Js.Promise.t(ChildProcess.spawnResult) => {
  let templateFile = getTemplatePath(template);
  transform("test");
};