module StatusCode = {
  [@bs.deriving jsConverter]
  type t =
    | [@bs.as 200] Ok
    | [@bs.as 201] Created
    | [@bs.as 202] Accepted
    | [@bs.as 203] NonAuthoritativeInformation
    | [@bs.as 204] NoContent
    | [@bs.as 205] ResetContent
    | [@bs.as 206] PartialContent
    | [@bs.as 207] MultiStatus
    | [@bs.as 208] AleadyReported
    | [@bs.as 226] IMUsed
    | [@bs.as 300] MultipleChoices
    | [@bs.as 301] MovedPermanently
    | [@bs.as 302] Found
    | [@bs.as 303] SeeOther
    | [@bs.as 304] NotModified
    | [@bs.as 305] UseProxy
    | [@bs.as 306] SwitchProxy
    | [@bs.as 307] TemporaryRedirect
    | [@bs.as 308] PermanentRedirect
    | [@bs.as 400] BadRequest
    | [@bs.as 401] Unauthorized
    | [@bs.as 402] PaymentRequired
    | [@bs.as 403] Forbidden
    | [@bs.as 404] NotFound
    | [@bs.as 405] MethodNotAllowed
    | [@bs.as 406] NotAcceptable
    | [@bs.as 407] ProxyAuthenticationRequired
    | [@bs.as 408] RequestTimeout
    | [@bs.as 409] Conflict
    | [@bs.as 410] Gone
    | [@bs.as 411] LengthRequired
    | [@bs.as 412] PreconditionFailed
    | [@bs.as 413] PayloadTooLarge
    | [@bs.as 414] UriTooLong
    | [@bs.as 415] UnsupportedMediaType
    | [@bs.as 416] RangeNotSatisfiable
    | [@bs.as 417] ExpectationFailed
    | [@bs.as 418] ImATeapot
    | [@bs.as 421] MisdirectedRequest
    | [@bs.as 422] UnprocessableEntity
    | [@bs.as 423] Locked
    | [@bs.as 424] FailedDependency
    | [@bs.as 426] UpgradeRequired
    | [@bs.as 428] PreconditionRequired
    | [@bs.as 429] TooManyRequests
    | [@bs.as 431] RequestHeaderFieldsTooLarge
    | [@bs.as 451] UnavailableForLegalReasons
    | [@bs.as 500] InternalServerError
    | [@bs.as 501] NotImplemented
    | [@bs.as 502] BadGateway
    | [@bs.as 503] ServiceUnavailable
    | [@bs.as 504] GatewayTimeout
    | [@bs.as 505] HttpVersionNotSupported
    | [@bs.as 506] VariantAlsoNegotiates
    | [@bs.as 507] InsufficientStorage
    | [@bs.as 508] LoopDetected
    | [@bs.as 510] NotExtended
    | [@bs.as 511] NetworkAuthenticationRequired;
  let fromInt = tFromJs;
  let toInt = tToJs;
};

type result =
  | Ok(Fetch.response)
  | FailureCode(StatusCode.t)
  | Failure;
type t = result;

let resolveSuccess = (~res: Fetch.response) => Ok(res)->Js.Promise.resolve;

let resolveFailure =
    (
      ~res: option(Fetch.response)=?,
      ~error: option(Js.Promise.error)=?,
      ~resource: string,
      unit,
    ) => {
  let status: option(StatusCode.t) =
    res
    ->Belt.Option.map(Fetch.Response.status)
    ->Belt.Option.flatMap(StatusCode.fromInt);

  Js.log(("Fetch failed => ", resource, status, error));

  let resultValue =
    switch (status) {
    | Some(value) => FailureCode(value)
    | None => Failure
    };

  Js.Promise.resolve(resultValue);
};

/**
 * A fetch wrapper that actually rejects the promise
 * when an error is received from the api
 * (source: https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API/Using_Fetch#Checking_that_the_fetch_was_successful)
 * "A fetch() promise will reject with a TypeError
 * when a network error is encountered or CORS is
 * misconfigured on the server-side, although this
 * usually means permission issues or similar — a 404
 * does not constitute a network error, for example.
 * An accurate check for a successful fetch() would
 * include checking that the promise resolved, then
 * checking that the Response.ok property has a value
 * of true."
 */
let fetchWrapper =
    (~resource: string, ~requestInit: Fetch.requestInit)
    : Js.Promise.t(result) => {
  Fetch.fetchWithInit(resource, requestInit)
  |> Js.Promise.then_(res
       // The ok read-only property of the Response interface
       // contains a Boolean stating whether the response was
       // successful (status in the range 200-299) or not.
       =>
         Fetch.Response.ok(res)
           ? resolveSuccess(~res) : resolveFailure(~res, ~resource, ())
       )
  |> Js.Promise.catch(error => resolveFailure(~error, ~resource, ()));
};

let get = (~resource: string) => {
  let requestInit = Fetch.RequestInit.make(~method_=Fetch.Get, ());
  fetchWrapper(~resource, ~requestInit);
};

let post = (~resource: string, ~body: Fetch.bodyInit) => {
  let requestInit = Fetch.RequestInit.make(~method_=Fetch.Post, ~body, ());
  fetchWrapper(~resource, ~requestInit);
};

let put = (~resource: string, ~body: Fetch.bodyInit) => {
  let requestInit = Fetch.RequestInit.make(~method_=Fetch.Put, ~body, ());
  fetchWrapper(~resource, ~requestInit);
};

let patch = (~resource: string, ~body: Fetch.bodyInit) => {
  let requestInit = Fetch.RequestInit.make(~method_=Fetch.Patch, ~body, ());
  fetchWrapper(~resource, ~requestInit);
};

let delete = (~resource: string) => {
  let requestInit = Fetch.RequestInit.make(~method_=Fetch.Delete, ());
  fetchWrapper(~resource, ~requestInit);
};