[@bs.set] external setWidthFloat: (Dom.element, float) => unit = "width";
[@bs.set] external setWidth: (Dom.element, int) => unit = "width";
[@bs.set] external setHeightFloat: (Dom.element, float) => unit = "height";
[@bs.set] external setHeight: (Dom.element, int) => unit = "height";

external windowElement: Webapi.Dom.Window.t => Webapi.Dom.Element.t =
  "%identity";