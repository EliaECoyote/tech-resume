open Belt2_option;

[@bs.get]
external get_webkitBackingStorePixelRatio:
  Webapi.Canvas.Canvas2d.t => option(float) =
  "webkitBackingStorePixelRatio";
[@bs.get]
external get_mozBackingStorePixelRatio:
  Webapi.Canvas.Canvas2d.t => option(float) =
  "mozBackingStorePixelRatio";
[@bs.get]
external get_msBackingStorePixelRatio:
  Webapi.Canvas.Canvas2d.t => option(float) =
  "msBackingStorePixelRatio";
[@bs.get]
external get_oBackingStorePixelRatio:
  Webapi.Canvas.Canvas2d.t => option(float) =
  "oBackingStorePixelRatio";
[@bs.get]
external get_backingStorePixelRatio: Webapi.Canvas.Canvas2d.t => option(float) =
  "backingStorePixelRatio";

let getContextPixelRatio = ctx =>
  get_webkitBackingStorePixelRatio(ctx)
  <??> get_mozBackingStorePixelRatio(ctx)
  <??> get_msBackingStorePixelRatio(ctx)
  <??> get_oBackingStorePixelRatio(ctx)
  <??> get_backingStorePixelRatio(ctx)
  |> Belt.Option.getWithDefault(_, 1.0);