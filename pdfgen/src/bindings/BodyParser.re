[@bs.module "body-parser"]
external json: unit => Express.Middleware.t = "json";

type textOptions = {
  defaultCharset: option(string),
  inflate: option(bool),
  limit: option(string),
  [@bs.as "type"]
  type_: option(string),
};

[@bs.module "body-parser"]
external text: unit => Express.Middleware.t = "text";

[@bs.module "body-parser"]
external textWithOptions: textOptions => Express.Middleware.t = "text";