module Types = {
  type pdf = option(Js.Typed_array.Int8Array.t);
  type canvasRef = React.Ref.t(Js.Nullable.t(Dom.element));
  type documentResult = Belt.Result.t(BsPdfjs.Document.t, Js.Exn.t);
};

module CanvasResizer = {
  let resize = (canvasElement, viewport) => {
    let devicePixelRatio =
      Global.devicePixelRatio |> Belt.Option.getWithDefault(_, 1.0);
    let backingStoreRatio =
      canvasElement
      |> Webapi.Canvas.CanvasElement.getContext2d
      |> Canvas2dHelper.getContextPixelRatio;
    let pixelRatio = devicePixelRatio /. backingStoreRatio;
    let newWidth = BsPdfjs.Viewport.width(viewport) |> Js.Math.floor_float;
    let newHeight = BsPdfjs.Viewport.height(viewport) |> Js.Math.floor_float;
    let scaleWidth = newWidth *. pixelRatio;
    let scaleHeight = newHeight *. pixelRatio;
    DomHelpers.setHeightFloat(canvasElement, scaleHeight);
    DomHelpers.setWidthFloat(canvasElement, scaleWidth);
    Js.log(("resizing....", canvasElement, scaleWidth, scaleHeight));
  };
};

module PDFLoader = {
  let loadPDFDocument = source =>
    source
    |> BsPdfjs.Global.getDocument(_, BsPdfjs.Global.inst)
    |> BsPdfjs.Global.DocumentLoadingTask.promise
    |> WonkaHelpers.fromPromise;

  let loadPDFPage = documentResult =>
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

  let load = pdfData =>
    Wonka.fromValue(pdfData)
    |> Wonka.map((. pdfData)
         // converts pdf typedArray to PDFjs source element
         => PdfJSHelpers.toPDFjsSource(pdfData))
    |> Wonka.switchMap((. source)
         // uses PDFjs source to load the pdf data
         => loadPDFDocument(source))
    |> Wonka.switchMap((. documentResult)
         // loads pdf page from documentResult
         => loadPDFPage(documentResult));
};

module PDFRenderer = {
  let renderInCanvas = (~canvasElement, page) => {
    let viewportParams =
      BsPdfjs.Page.viewportParams(~scale=1.0, ~rotate=0.0, ());
    let viewport = BsPdfjs.Page.getViewport(page, viewportParams);
    let canvasContext =
      Webapi.Canvas.CanvasElement.getContext2d(canvasElement);
    page
    |> BsPdfjs.Page.render(_, ~canvasContext, ~viewport, ~transform=None)
    |> BsPdfjs.RenderTask.promise
    |> WonkaHelpers.fromPromise
    |> WonkaHelpers.Result.tapLogError(~message="[Pdf render]")
    |> Wonka.map((. result) =>
         switch (result) {
         | Belt.Result.Error(_error) =>
           Belt.Result.Error("Something went wrong during PDF page render")
         | Belt.Result.Ok(_) => Belt.Result.Ok(viewport)
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
      switch (pdf) {
      | Some(pdf) =>
        Wonka.fromValue(pdf)
        |> Wonka.switchMap((. pdf) => PDFLoader.load(pdf))
        |> Wonka.switchMap((. pageResult) =>
             PDFRenderer.render(canvasRef, pageResult)
           )
        |> Wonka.subscribe((. result) => {
             let canvasElement =
               React.Ref.current(canvasRef) |> Js.Nullable.toOption;
             switch (canvasElement, result) {
             | (Some(canvasElement), Belt.Result.Ok(viewport)) =>
               CanvasResizer.resize(canvasElement, viewport)
             // TODO: this should be notified to the user as a toast or similar
             | (_, Belt.Result.Error(errorMsg)) =>
               Js.Console.error(errorMsg)
             // TODO: this should be notified to the user as a toast or similar
             | (None, _) => Js.Console.error("canvas element not found")
             };
           })
        |> WonkaHelpers.getEffectCleanup
      | None => None
      },
    [|pdf|],
  );
  ();
};
/*
 const devicePixelRatio = window.devicePixelRatio || 1

 const ctx = canvas[0].getContext('2d')
 const backingStoreRatio =
   ctx.webkitBackingStorePixelRatio ||
   ctx.mozBackingStorePixelRatio ||
   ctx.msBackingStorePixelRatio ||
   ctx.oBackingStorePixelRatio ||
   ctx.backingStorePixelRatio ||
   1
 const pixelRatio = devicePixelRatio / backingStoreRatio

 const scaledWidth = (Math.floor(viewport.width) * pixelRatio) | 0
 const scaledHeight = (Math.floor(viewport.height) * pixelRatio) | 0

 const newWidth = Math.floor(viewport.width)
 const newHeight = Math.floor(viewport.height)

 canvas[0].height = scaledHeight
 canvas[0].width = scaledWidth
 */