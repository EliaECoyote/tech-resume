[@bs.send.pipe: Express.Response.t]
external download: (~path: string, ~filename: string=?) => Express.complete =
  "download";