module Types = {
  type pdf = option(Fetch.arrayBuffer);
  type canvasRef = React.Ref.t(Js.Nullable.t(Dom.element));
  type documentResult = Belt.Result.t(BsPdfjs.Document.t, Js.Exn.t);
};

module PDFLoader = {
  let loadFromDocumentResult = documentResult =>
    switch (documentResult) {
    | Belt.Result.Ok(page) =>
      page
      // loads first pdf page
      |> BsPdfjs.Document.getPage(1)
      // converts promise to Wonka source
      |> WonkaHelpers.fromPromise
      |> WonkaHelpers.Result.tapLogError(~message="[Pdf page load]")
      |> Wonka.map((. result) =>
           switch (result) {
           | Belt.Result.Ok(data) => Belt.Result.Ok(data)
           | Belt.Result.Error(_error) =>
             Belt.Result.Error("Something went wrong during PDF page loading")
           }
         )
    | Belt.Result.Error(_error) =>
      Belt.Result.Error("Something went wrong during PDF document loading")
      |> Wonka.fromValue
    };

  let load = (pdf: Fetch.arrayBuffer) =>
    Wonka.fromValue(pdf)
    |> Wonka.map((. pdf) =>
         pdf
         // converts buffer to PDFjs source element
         |> PdfJSHelpers.toTypedArray
         |> PdfJSHelpers.toPDFjsSource
       )
    |> Wonka.switchMap((. source) =>
         source
         // uses PDFjs source to load the pdf data
         |> BsPdfjs.Global.getDocument(_, BsPdfjs.Global.inst)
         |> BsPdfjs.Global.DocumentLoadingTask.promise
         |> WonkaHelpers.fromPromise
       )
    |> Wonka.switchMap((. documentResult)
         // loads pdf page from documentResult
         => loadFromDocumentResult(documentResult));
};

module PDFRenderer = {
  let renderInCanvas = (~canvasElement, page) => {
    let scale = BsPdfjs.Viewport.ScalePageFit;
    let viewportParams =
      BsPdfjs.Page.viewportParams(~scale=1.0, ~rotate=1.0, ());
    let viewport = BsPdfjs.Page.getViewport(page, viewportParams);
    BsPdfjs.Viewport.convertToViewportPoint(50.0, 50.0);
    let canvasContext =
      Webapi.Canvas.CanvasElement.getContext2d(canvasElement);
    page
    |> BsPdfjs.Page.render(_, ~canvasContext, ~viewport, ~transform=None)
    |> BsPdfjs.RenderTask.promise
    |> WonkaHelpers.fromPromise
    |> WonkaHelpers.Result.tapLogError(~message="[Pdf render]")
    |> Wonka.map((. result) =>
         switch (result) {
         | Belt.Result.Error(error) =>
           Belt.Result.Error("Something went wrong during PDF page render")
         | Belt.Result.Ok(data) => Belt.Result.Ok(data)
         }
       );
  };

  let render = (canvasRef, pageResult) => {
    let canvas = canvasRef |> React.Ref.current |> Js.Nullable.toOption;
    switch (canvas, pageResult) {
    | (Some(canvasElement), Belt.Result.Ok(page)) =>
      renderInCanvas(~canvasElement, page)
    | (_, Belt.Result.Error(message)) =>
      Belt.Result.Error(message) |> Wonka.fromValue
    | _ =>
      Belt.Result.Error("Something went wrong during PDF generation")
      |> Wonka.fromValue
    };
  };
};

let hook = (~pdf: Types.pdf, ~canvasRef: Types.canvasRef) => {
  React.useEffect1(
    () =>
      pdf
      |> Belt.Option.flatMap(_, pdf =>
           pdf
           |> PDFLoader.load
           |> PDFRenderer.render(canvasRef)
           |> Wonka.subscribe((. result) => Js.log(result))
           |> WonkaHelpers.getEffectCleanup
         ),
    [|pdf|],
  );
};