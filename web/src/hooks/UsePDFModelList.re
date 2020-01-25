module Types = {
  type pdf = option(Js.Typed_array.Int8Array.t);
  type pdfValidModel = {
    id: string,
    logicalWidth: float,
    logicalHeight: float,
    contextScale: float,
    page: BsPdfjs.Page.t,
  };
  type pdfModel = Belt.Result.t(pdfValidModel, string);
};

module PDFDimensions = {
  let multiplyFloat = (a1, a2) => a1 *. a2;

  /**
   * retrieves the canvas *logical* size by multiplying the result
   * of the viewport dimensions with the pixelRatio
   */
  let getCanvasLogicalDimensions = (viewport, pixelRatio) => {
    let width =
      viewport
      |> BsPdfjs.Viewport.width
      |> Js.Math.floor_float
      |> multiplyFloat(pixelRatio)
      |> Js.Math.floor_float;
    let height =
      viewport
      |> BsPdfjs.Viewport.height
      |> Js.Math.floor_float
      |> multiplyFloat(pixelRatio)
      |> Js.Math.floor_float;
    (width, height);
  };
};

module PDFLoader = {
  let loadDocument = source =>
    source
    |> BsPdfjs.Global.getDocument(_, BsPdfjs.Global.inst)
    |> BsPdfjs.Global.DocumentLoadingTask.promise
    |> WonkaHelpers.fromPromise;

  let loadPage = (document, pageIndex) =>
    document
    |> BsPdfjs.Document.getPage(pageIndex)
    |> WonkaHelpers.fromPromise
    |> WonkaHelpers.Result.tapLogError(
         ~message={j|[Pdf page $pageIndex load]|j},
       )
    |> Wonka.map((. result) =>
         switch (result) {
         | Belt.Result.Ok(data) => Belt.Result.Ok(data)
         | Belt.Result.Error(_error) =>
           Belt.Result.Error("Something went wrong during PDF page loading")
         }
       );

  let loadPages = documentResult =>
    switch (documentResult) {
    | Belt.Result.Ok(document) =>
      document
      |> BsPdfjs.Document.getNumPages
      |> Array.make(_, 0)
      |> Array.mapi((index, _) => index + 1)
      |> Wonka.fromValue
      |> Wonka.switchMap((. pagesIndexes) =>
           pagesIndexes
           |> Array.map(loadPage(document))
           |> WonkaHelpers.combineArray
           |> Wonka.map((. array) => Belt.Result.Ok(array))
         )
      |> Wonka.take(1)
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
         => loadDocument(source))
    |> Wonka.switchMap((. documentResult)
         // loads pdf page from documentResult
         => loadPages(documentResult));
};

module PDFModel = {
  let getFromValidPage = page => {
    let viewportParams =
      BsPdfjs.Page.viewportParams(~scale=1.0, ~rotate=0.0, ());
    let viewport = BsPdfjs.Page.getViewport(page, viewportParams);
    let dpr = Belt.Option.getWithDefault(Global.devicePixelRatio, 1.0);
    let (width, height) =
      PDFDimensions.getCanvasLogicalDimensions(viewport, dpr);
    let model: Types.pdfValidModel = {
      id: Uuid.V4.make(),
      logicalWidth: width,
      logicalHeight: height,
      contextScale: dpr,
      page,
    };
    model;
  };

  let get = pageResult =>
    switch (pageResult) {
    | Belt.Result.Ok(page) => Belt.Result.Ok(getFromValidPage(page))
    | Belt.Result.Error(message) => Belt.Result.Error(message)
    };
};

let hook = (~pdf: Types.pdf) => {
  open AsyncTask;
  let (state, sendEvent) = UseMachine.hook(~reducer, ~initialValue=Idle);
  React.useEffect1(
    () =>
      switch (pdf) {
      | Some(pdf) =>
        Wonka.fromValue(pdf)
        |> Wonka.onStart((.) => sendEvent(LoadData))
        |> Wonka.switchMap((. pdf) => PDFLoader.load(pdf))
        |> Wonka.subscribe((. result) => {
             switch (result) {
             | Belt.Result.Ok(pages) =>
               pages
               |> Array.map(PDFModel.get)
               |> (models => LoadSuccess(models))
               |> sendEvent
             | Belt.Result.Error(errorMsg) =>
               sendEvent(LoadFailed);
               Js.Console.error(errorMsg);
               ();
             }
           })
        |> WonkaHelpers.getEffectCleanup
      | None =>
        // TODO: add event to move to *idle* state here
        None
      },
    [|pdf|],
  );
  state;
};