[@bs.deriving abstract]
type htmlAttributes = {lang: string};
type bodyAttributes;
type titleAttributes;
type base;
[@bs.deriving abstract]
type meta = {
  [@bs.optional]
  name: string,
  [@bs.optional]
  property: string,
  [@bs.optional]
  content: string,
};
type link;
type style;
type script;
type noscript;

[@bs.module "react-helmet"] [@react.component]
external make:
  (
    ~defer: bool=?,
    ~encodeSpecialCharacters: bool=?,
    ~onChangeClientState: unit => unit=?,
    ~htmlAttributes: htmlAttributes=?,
    ~bodyAttributes: bodyAttributes=?,
    ~titleAttributes: titleAttributes=?,
    ~defaultTitle: string=?,
    ~titleTemplate: string=?,
    ~base: base=?,
    ~title: string=?,
    ~meta: array(meta)=?,
    ~link: array(link)=?,
    ~style: array(style)=?,
    ~script: array(script)=?,
    ~noscript: array(noscript)=?,
    ~children: React.element=?
  ) =>
  React.element =
  "Helmet";