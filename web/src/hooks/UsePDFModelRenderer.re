open UsePDFModelList.Types;

let render = (model, canvasElement) => {
  let viewportParams =
    BsPdfjs.Page.viewportParams(~scale=1.0, ~rotate=0.0, ());
  let viewport = BsPdfjs.Page.getViewport(model.page, viewportParams);
  let canvasContext = Webapi.Canvas.CanvasElement.getContext2d(canvasElement);
  Webapi.Canvas.Canvas2d.scale(
    ~x=model.contextScale,
    ~y=model.contextScale,
    canvasContext,
  );
  BsPdfjs.Page.render(model.page, ~canvasContext, ~viewport, ~transform=None)
  |> BsPdfjs.RenderTask.promise
  |> Belt2.Wonka.fromPromiseSafe
  |> Belt2.Wonka.Result.tapLogError(~message="[Pdf render]")
  |> Wonka.map((. result) =>
       switch (result) {
       | Belt.Result.Error(_error) =>
         Belt.Result.Error("Something went wrong during PDF page render")
       | Belt.Result.Ok(_) => Belt.Result.Ok()
       }
     );
};

let hook =
    (~model: Belt.Result.t(UsePDFModelList.Types.pdfValidModel, string)) => {
  React.useEffect1(
    () =>
      switch (model) {
      | Belt.Result.Ok(model) =>
        DocumentRe.getElementById(model.id, Webapi.Dom.document)
        |> Belt.Option.flatMap(_, canvasElement =>
             render(model, canvasElement)
             |> Wonka.publish
             |> Belt2.Wonka.getEffectCleanup
           )
      | Belt.Result.Error(_) => None
      },
    [|model|],
  );
  ();
};