let mainMiddleware =
  Express.Middleware.from((next, req) => {
    Test.runTest();
    Express.Response.sendString("hello world");
  });