type iFrameElement;

[@bs.get]
external getContentWindow: Dom.element => Dom.window = "contentWindow";

[@bs.send] external print: Dom.window => unit = "print";

external toIframeElement: Dom.element => Dom.element_like(iFrameElement) =
  "%identity";