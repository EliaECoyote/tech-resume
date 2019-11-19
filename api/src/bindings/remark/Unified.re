module Types = {
  type processor;
  type plugin;
  type processCallback = (Js.nullable(Js.Exn.t), VFile.vFile) => unit;
};

/**
 * Clone current processor
 *
 * @returns New unfrozen processor which is configured to function the same as its ancestor.
 * But when the descendant processor is configured in the future it does not affect the ancestral processor.
 */
[@bs.module]
external unified: unit => Types.processor = "unified";

[@bs.send]
external use: (Types.processor, Types.plugin) => Types.processor = "use";

/**
 * Process the given representation of a file as configured on the processor. The process invokes `parse`, `run`, and `stringify` internally.
 * @param file `VFile` or anything which can be given to `vfile()`
 * @param done Invoked when the process is complete. Invoked with a fatal error, if any, and the VFile.
 */
[@bs.send]
external process: (Types.processor, string, Types.processCallback) => unit =
  "process";