[@bs.module] external doc: Unified.Types.plugin = "rehype-document";

[@bs.deriving abstract]
type options = {
  /**
   * Text to use as title (string, default: name of file, if any).
   */
  [@bs.optional]
  title: string,
  /**
   * Natural language of document (string, default: 'en'). should be a BCP 47 language tag.
   */
  [@bs.optional]
  language: string,
  /**
   * Whether to insert a meta[viewport] (boolean, default: true).
   */
  [@bs.optional]
  responsive: bool,
  /**
   * Doctype to use (string, default: '5').
   */
  [@bs.optional]
  doctype: string,
  /**
   * CSS to include in head in <style> elements (string or Array.<string>, default: []).
   */
  [@bs.optional]
  style: Js.Array.t(string),
  /**
   * Links to stylesheets to include in head (string or Array.<string>, default: []).
   */
  [@bs.optional]
  css: Js.Array.t(string),
  /**
   * Metadata to include in head (Object or Array.<Object>, default: []).
   * Each object is passed as properties to hastscript with a meta element.
   */
  [@bs.optional]
  meta: Js.Dict.t(string),
  /**
   * Link tags to include in head (Object or Array.<Object>, default: []).
   * Each object is passed as properties to hastscript with a link element.
   */
  [@bs.optional]
  link: Js.Dict.t(string),
  /**
   * Scripts to include at end of body (string or Array.<string>, default: []).
   */
  [@bs.optional]
  js: Js.Array.t(string),
};