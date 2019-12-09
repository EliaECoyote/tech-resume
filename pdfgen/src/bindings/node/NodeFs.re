// Node Fs bindings
// Docs: https://nodejs.org/dist/latest-v10.x/docs/api/fs.html

module Stat = {
  [@bs.deriving abstract]
  type statsOptions = {bigint: bool};

  [@bs.deriving abstract]
  type stats = {
    dev: int,
    ino: int,
    mode: int,
    nlink: int,
    uid: int,
    gid: int,
    rdev: int,
    size: int,
    blksize: int,
    blocks: int,
    atimeMs: float,
    mtimeMs: float,
    ctimeMs: float,
    birthtimeMs: float,
    atime: string,
    mtime: string,
    ctime: string,
    birthtime: string,
  };

  [@bs.module "fs"] [@bs.scope "promises"]
  external stat:
    (~path: string, ~options: statsOptions=?) => Js.Promise.t(stats) =
    "stat";

  let stat = (~path: string, ~options: option(statsOptions), ()) =>
    stat(~path, ~options?)->WonkaHelpers.fromPromise;
};

module Unlink = {
  [@bs.module "fs"] [@bs.scope "promises"]
  external unlink: (~path: string) => Js.Promise.t(unit) = "unlink";

  let unlink = (~path) => unlink(~path)->WonkaHelpers.fromPromise;
};

let wonkaStat = Stat.stat;
let wonkaUnlink = Unlink.unlink;