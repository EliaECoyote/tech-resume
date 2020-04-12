open Express;

module Types = {
  type origin = [
    | `All
    | `Str(string)
    | `Arr(array(string))
    | `Rgx(Js.Re.t)
  ];

  type corsOptions = {
    origin,
    credentials: bool,
    preflightContinue: bool,
    maxAge: option(int),
    methods: array(Request.httpMethod),
    allowedHeaders: array(string),
    exposedHeaders: array(string),
    optionsSuccessStatus: Response.StatusCode.t,
  };

  type header = {
    key: string,
    value: string,
  };
};

let defaultOptions: Types.corsOptions = {
  origin: `All,
  credentials: false,
  maxAge: None,
  methods: [|
    Request.Get,
    Request.Head,
    Request.Put,
    Request.Patch,
    Request.Post,
    Request.Delete,
  |],
  allowedHeaders: [||],
  exposedHeaders: [||],
  preflightContinue: false,
  optionsSuccessStatus: Response.StatusCode.UnprocessableEntity,
};

let httpMethodToString = (method: Request.httpMethod) =>
  switch (method) {
  | Request.Get => "GET"
  | Request.Head => "HEAD"
  | Request.Put => "PUT"
  | Request.Patch => "PATCH"
  | Request.Post => "POST"
  | Request.Delete => "DELETE"
  | Request.Trace => "TRACE"
  | Request.Connect => "CONNECT"
  | Request.Options => "OPTIONS"
  };

let buildVaryHeader = (value): Types.header => {key: "Vary", value};

module Origin = {
  let buildAllowOriginHeader = (value): Types.header => {
    key: "Access-Control-Allow-Origin",
    value,
  };
  let buildOriginVaryHeader = () => buildVaryHeader("Origin");

  let configure =
      (options: Types.corsOptions, req: Request.t): array(Types.header) => {
    let requestOrigin = Request.originalUrl(req);
    let isSameOrigin = origin => origin === requestOrigin;

    let headers: list(Types.header) =
      switch (options.origin) {
      | `All => [buildAllowOriginHeader("*")]
      | `Str(value) => [
          buildAllowOriginHeader(value),
          buildOriginVaryHeader(),
        ]
      | `Arr(value) =>
        let isAllowed = value |> Js.Array.some(isSameOrigin);
        let isSameOriginValue = isAllowed ? requestOrigin : "false";
        [buildAllowOriginHeader(isSameOriginValue), buildOriginVaryHeader()];
      | `Rgx(value) =>
        let isAllowed = requestOrigin |> Js.Re.test_(value);
        let isSameOriginValue = isAllowed ? requestOrigin : "false";
        [buildAllowOriginHeader(isSameOriginValue), buildOriginVaryHeader()];
      };

    Array.of_list(headers);
  };
};

module Methods = {
  let buildControlAllowHeader = (value: string): Types.header => {
    key: "Access-Control-Allow-Methods",
    value,
  };

  let configure = (options: Types.corsOptions): array(Types.header) => {
    let strMethods =
      options.methods
      |> Array.map(httpMethodToString)
      |> Js.Array.joinWith(",");

    [|buildControlAllowHeader(strMethods)|];
  };
};

module Credentials = {
  let buildAllowCredentialsHeader = (): Types.header => {
    key: "Access-Control-Allow-Credentials",
    value: "true",
  };

  let configure = (options: Types.corsOptions): array(Types.header) =>
    options.credentials ? [|buildAllowCredentialsHeader()|] : [||];
};

module AllowedHeaders = {
  let configure = (options: Types.corsOptions): array(Types.header) =>
    switch (options.allowedHeaders) {
    | [||] => [|buildVaryHeader("Access-Control-Request-Headers")|]
    | _ =>
      let value = options.allowedHeaders |> Js.Array.joinWith(",");
      [|buildVaryHeader(value)|];
    };
};

module ExposedHeaders = {
  let buildAccessControllExposeHeadersHeader = (value: string): Types.header => {
    key: "Access-Control-Expose-Headers",
    value,
  };

  let configure = (options: Types.corsOptions): array(Types.header) => {
    switch (options.exposedHeaders) {
    | [||] => [||]
    | _ =>
      let value = options.exposedHeaders |> Js.Array.joinWith(",");
      [|buildAccessControllExposeHeadersHeader(value)|];
    };
  };
};

module MaxAgeHeaders = {
  let buildMaxAgeHeader = (value: string): Types.header => {
    key: "Access-Control-Max-Age",
    value,
  };

  let configure = (options: Types.corsOptions): array(Types.header) =>
    switch (options.maxAge) {
    | None => [||]
    | Some(value) =>
      let maxAgeHeader = string_of_int(value)->buildMaxAgeHeader;
      [|maxAgeHeader|];
    };
};

let setResponseHeaders = (headers: array(Types.header), response: Response.t) => {
  let setSingleResponseHeader = (header: Types.header) => {
    response |> Response.setHeader(header.key, header.value) |> (_ => ());
  };
  headers |> Array.iter(setSingleResponseHeader);
};

let middlewareFactory = (~options: Types.corsOptions=defaultOptions, ()) =>
  Middleware.from((next: Middleware.next, req: Request.t, res: Response.t) => {
    let method = Request.httpMethod(req);
    switch (method) {
    | Request.Options =>
      let headers =
        Array.concat([
          Origin.configure(options, req),
          Credentials.configure(options),
          Methods.configure(options),
          AllowedHeaders.configure(options),
          MaxAgeHeaders.configure(options),
          ExposedHeaders.configure(options),
        ]);
      setResponseHeaders(headers, res);

      options.preflightContinue
        ? res |> next(Next.middleware)
        : res
          |> Response.setHeader("Content-Length", "0")
          |> Response.rawStatus(204)
          |> Response.end_;

    | _ =>
      let headers =
        Array.concat([
          Origin.configure(options, req),
          Credentials.configure(options),
          ExposedHeaders.configure(options),
        ]);
      setResponseHeaders(headers, res);
      res |> next(Next.middleware);
    };
  });