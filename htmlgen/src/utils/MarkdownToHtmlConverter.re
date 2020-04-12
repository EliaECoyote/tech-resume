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
      "https://fonts.googleapis.com/css?family=Roboto:400,400i,700,700i&display=swap",
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
          color: #626262;
          text-align: center;
          margin: 4px;
        }
        blockquote {
          text-align: center;
          margin: 8px;
        }
        h2 {
          color: #2d6139;
          margin: 6px;
        }
        h2::before {
          content: '';
          background: #8bab5d;
          height: 8px;
          margin-right: 6px;
          width: 16%;
          display: inline-block;
        }
        p {
          margin: 4px
        }
        ul {
          margin: 8px;
        }
        li {
          line-height: 1.35rem
        }
        a {
          color: #2d6139;
          text-decoration: none;
        }
      |j},
    |]
  };
};

let getErrorMessage = (error: Js.Exn.t) =>
  Js.Exn.message(error)
  |> Belt.Option.getWithDefault(_, "generic converter error");

/**
 * converts markdown to html code. The **css** parameter will
 * be embedded as a style tag on the *head* of the document
 */
let transform = (~src: string, ~style: array(string), ~css: array(string)) =>
  Wonka.make((. observer: Wonka.Types.observerT(result)) => {
    let cancelledSubject = Wonka.makeSubject();
    let subscription = ref(None);
    let parseOptions = RemarkParse.options(~commonmark=true, ());
    let documentOptions =
      RemarkDocument.options(~title="resume document", ~style, ~css, ());
    Unified.unified()
    |> Unified.use(
         _,
         ~plugin=RemarkParse.markdown,
         ~options=parseOptions,
         (),
       )
    |> Unified.use(_, ~plugin=RemarkRehype.remark2rehype, ())
    |> Unified.use(
         _,
         ~plugin=RemarkDocument.doc,
         ~options=documentOptions,
         (),
       )
    |> Unified.use(_, ~plugin=RehypeFormat.format, ())
    |> Unified.use(_, ~plugin=RehypeStringify.html, ())
    |> Unified.process(
         _,
         src,
         (error, data) => {
           let resultSource =
             switch (Js.Nullable.toOption(error)) {
             | Some(value) =>
               Wonka.fromValue(value)
               |> Wonka.tap((. value) => Js.Console.error(value))
               |> Wonka.map((. value) => getErrorMessage(value))
               |> Wonka.map((. value) => Error(value))
             | None =>
               VFile.contentsGet(data)
               |> Wonka.fromValue
               |> Wonka.map((. value) => Ok(value))
             };
           subscription :=
             resultSource
             |> Wonka.takeUntil(cancelledSubject.source)
             |> Wonka.subscribe((. value) => {
                  observer.next(value);
                  observer.complete();
                })
             |> (value => Some(value));
         },
       );
    (.) => {
      let _ =
        subscription^
        |> Belt.Option.map(_, subscription => subscription.unsubscribe());
      cancelledSubject.next();
      cancelledSubject.complete();
    };
  });

/**
 * runs the md 2 html converter. The **template** parameter specifies
 * which template will be used to style the html document
 */
let run = (~src: string, ~template: template) => {
  let style = getTemplateStyle(template);
  let css = getTemplateCssResources(template);
  transform(~src, ~style, ~css);
};