[@bs.module] external markdown: Unified.Types.plugin = "remark-parse";

[@bs.deriving abstract]
type options = {
  [@bs.optional]
  gfm: bool,
  [@bs.optional]
  commonmark: bool,
  [@bs.optional]
  footnotes: bool,
  [@bs.optional]
  pedantic: bool,
  [@bs.optional]
  blocks: array(string),
};