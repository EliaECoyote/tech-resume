[@bs.module] external markdown: Unified.Types.plugin = "remark-parse";

type optionsT = {
  gfm: bool,
  commonmark: bool,
  footnotes: bool,
  pedantic: bool,
  blocks: array(string),
};