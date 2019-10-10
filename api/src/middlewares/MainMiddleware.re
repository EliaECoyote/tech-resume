let mainMiddleware =
  Express.Middleware.from((next, req) => {
    Pandoc.run();
    Express.Response.sendString("hello world");
  });