module Types = {
  type pdf = option(Js.Typed_array.Int8Array.t);
  type canvasRef = React.Ref.t(Js.Nullable.t(Dom.element));
  type documentResult = Belt.Result.t(BsPdfjs.Document.t, Js.Exn.t);
};

module CanvasResizer = {
  let divideFloat = (a1, a2) => a1 /. a2;
  let multiplyFloat = (a1, a2) => a1 *. a2;
  /**
   * calculates the canvas height by following this proportion:
   * pdfWidth : canvasWidth = pdfHeight : canvasHeight
   */
  let getCanvasHeight = (pdfWidth, pdfHeight, canvasWidth) =>
    pdfHeight *. canvasWidth /. pdfWidth;

  /**
   * calculates the scale by following this proportion:
   * pdfWidth : initialScale = canvasWidth : x
   * (the initial pdf scale is always 1)
   */
  let getPixelRatio = (dpr, canvasContext) =>
    canvasContext |> Canvas2dHelper.getContextPixelRatio |> divideFloat(dpr);

  /**
   * updates the canvas *logical* size with the result
   * of the viewport dimensions * pixelRatio
   */
  let updateCanvasLogicalSize = (viewport, pixelRatio, canvasElement) => {
    viewport
    |> BsPdfjs.Viewport.width
    |> Js.Math.floor_float
    |> multiplyFloat(pixelRatio)
    |> Js.Math.floor_float
    |> DomHelpers.setWidthFloat(canvasElement);
    viewport
    |> BsPdfjs.Viewport.height
    |> Js.Math.floor_float
    |> multiplyFloat(pixelRatio)
    |> Js.Math.floor_float
    |> DomHelpers.setHeightFloat(canvasElement);
  };

  let resize = (~canvasElement, ~canvasContext, ~viewport) => {
    let dpr = Global.devicePixelRatio |> Belt.Option.getWithDefault(_, 1.0);
    let pixelRatio = getPixelRatio(dpr, canvasContext);
    updateCanvasLogicalSize(viewport, pixelRatio, canvasElement);
    Webapi.Canvas.Canvas2d.scale(~x=pixelRatio, ~y=pixelRatio, canvasContext);
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
    CanvasResizer.resize(~canvasElement, ~canvasContext, ~viewport);
    page
    |> BsPdfjs.Page.render(_, ~canvasContext, ~viewport, ~transform=None)
    |> BsPdfjs.RenderTask.promise
    |> WonkaHelpers.fromPromise
    |> WonkaHelpers.Result.tapLogError(~message="[Pdf render]")
    |> Wonka.map((. result) =>
         switch (result) {
         | Belt.Result.Error(_error) =>
           Belt.Result.Error("Something went wrong during PDF page render")
         | Belt.Result.Ok(_) => Belt.Result.Ok()
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
             switch (result) {
             | Belt.Result.Ok(_) => ()
             // TODO: this should be notified to the user as a toast or similar
             | Belt.Result.Error(errorMsg) => Js.Console.error(errorMsg)
             }
           })
        |> WonkaHelpers.getEffectCleanup
      | None => None
      },
    [|pdf|],
  );
  ();
};