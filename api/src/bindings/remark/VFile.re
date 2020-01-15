[@bs.deriving abstract]
type vFileOptions = {
  [@bs.optional]
  contents: NodeHelper.StringBuffer.t,
  [@bs.optional]
  path: string,
  [@bs.optional]
  basename: string,
  [@bs.optional]
  stem: string,
  [@bs.optional]
  extname: string,
  [@bs.optional]
  dirname: string,
  [@bs.optional]
  cwd: string,
};

[@bs.deriving abstract]
type vFile = {
  /**
   * List of file-paths the file moved between.
   */
  history: Js.Array.t(string),
  /**
   * Raw value.
   */
  contents: NodeHelper.StringBuffer.t,
  /**
   * Base of `path`.
   * Defaults to `process.cwd()`.
   */
  cwd: string,
  /**
   * Path of `vfile`.
   * Cannot be nullified.
   */
  [@bs.optional]
  path: string,
  /**
   * Path to parent directory of `vfile`.
   * Cannot be set if there's no `path` yet.
   */
  [@bs.optional]
  dirname: string,
  /**
   * Current name (including extension) of `vfile`.
   * Cannot contain path separators.
   * Cannot be nullified either (use `file.path = file.dirname` instead).
   */
  [@bs.optional]
  basename: string,
  /**
   * Name (without extension) of `vfile`.
   * Cannot be nullified, and cannot contain path separators.
   */
  [@bs.optional]
  stem: string,
  /**
   * Extension (with dot) of `vfile`.
   * Cannot be set if there's no `path` yet and cannot contain path separators.
   */
  [@bs.optional]
  extname: string,
};

let contentsGet = (vFile: vFile) =>
  vFile |> contentsGet |> NodeHelper.StringBuffer.getStringValue;