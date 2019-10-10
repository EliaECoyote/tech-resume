let mainMiddleware =
  Express.Middleware.from((next, req) => {
    Pandoc.run(~src="# Test", ~fromKind=Pandoc.Markdown, ~toKind=Pandoc.Html);
    Express.Response.sendString("hello world");
  });