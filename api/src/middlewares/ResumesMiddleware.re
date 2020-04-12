let middleware =
  Express.Middleware.from((next, _req, res) => {
    next(Express.Next.middleware, res)
  });