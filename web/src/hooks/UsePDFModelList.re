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
    |> XWonka.fromPromiseSafe;

  let loadPage = (document, pageIndex) =>
    document
    |> BsPdfjs.Document.getPage(pageIndex)
    |> XWonka.fromPromiseSafe
    |> XWonka.tapLogError(~message={j|[Pdf page $pageIndex load]|j})
    |> XWonka.map(result =>
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
      |> XWonka.fromValue
      |> XWonka.switchMap(pagesIndexes =>
           pagesIndexes
           |> Array.map(loadPage(document))
           |> XWonka.combineArray
           |> XWonka.map(array => Belt.Result.Ok(array))
         )
      |> XWonka.take(1)
    | Belt.Result.Error(_error) =>
      Belt.Result.Error("Something went wrong during PDF document loading")
      |> XWonka.fromValue
    };

  let load = pdfData =>
    XWonka.fromValue(pdfData)
    // converts pdf typedArray to PDFjs source element
    |> XWonka.map(Belt2.PdfJs.toPdfJsSource)
    // uses PDFjs source to load the pdf data
    |> XWonka.switchMap(loadDocument)
    // loads pdf page from documentResult
    |> XWonka.switchMap(loadPages);
};

module PDFModel = {
  let getFromValidPage = page => {
    let viewportParams =
      BsPdfjs.Page.viewportParams(~scale=1.0, ~rotate=0.0, ());
    let viewport = BsPdfjs.Page.getViewport(page, viewportParams);
    let dpr = Belt.Option.getWithDefault(Js2.Global.devicePixelRatio, 1.0);
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

let hook = (~pdfRequestState: AsyncTask.status(Js_typed_array.Int8Array.t)) => {
  let (state, sendEvent) =
    UseMachine.hook(~reducer=AsyncTask.reducer, ~initialValue=Idle);
  React.useEffect1(
    () =>
      switch (pdfRequestState) {
      | AsyncTask.Fetching =>
        sendEvent(LoadData);
        None;
      | AsyncTask.Idle
      | AsyncTask.Error =>
        sendEvent(Reset);
        None;
      | AsyncTask.Success(pdf) =>
        XWonka.fromValue(pdf)
        |> XWonka.onStart(() => sendEvent(LoadData))
        |> XWonka.switchMap(PDFLoader.load)
        |> XWonka.subscribe(result =>
             switch (result) {
             | Belt.Result.Ok(pages) =>
               pages
               |> Array.map(PDFModel.get)
               |> (models => AsyncTask.LoadSuccess(models))
               |> sendEvent
             | Belt.Result.Error(errorMsg) =>
               sendEvent(LoadFailed);
               Js.Console.error(errorMsg);
               ();
             }
           )
        |> XWonka.getEffectCleanup
      },
    [|pdfRequestState|],
  );
  state;
};