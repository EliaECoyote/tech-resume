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

module Convert = {
  external toTypedArray: Fetch.arrayBuffer => Js_typed_array.Int8Array.t =
    "%identity";
};

type result('kind) =
  | Ok('kind)
  | FailureCode(StatusCode.t)
  | Failure;

type t = result(Fetch.response);

/**
 * A fetch wrapper that handles and returns failure
 * in case an error is received from the api
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
let fetchWrapper = (~resource: string, ~requestInit: Fetch.requestInit) => {
  let cancelled = ref(false);

  // manually creating a wonka source, in order to handle correctly
  // the Fetch result promise errors
  XWonka.make(observer => {
    // utils fns used to avoid invoking *next* and *complete
    // observer methods when the subscription gets cancelled
    let observerNext = value =>
      if (! cancelled^) {
        observer.next(value);
      };
    let observerComplete = () =>
      if (! cancelled^) {
        observer.complete();
      };

    let _ =
      Fetch.fetchWithInit(resource, requestInit)
      |> Js.Promise.then_(res => {
           // The ok read-only property of the Response interface
           // contains a Boolean stating whether the response was
           // successful (status in the range 200-299) or not.
           Fetch.Response.ok(res)
             ? Ok(res)->observerNext
             : {
               res
               ->Fetch.Response.status
               ->StatusCode.fromInt
               ->Belt.Option.map(code => FailureCode(code))
               ->Belt.Option.getWithDefault(Failure)
               ->observerNext;
             };
           observerComplete();
           Js.Promise.resolve();
         })
      |> Js.Promise.catch(error => {
           Js.log(error);
           observerNext(Failure);
           observerComplete();
           Js.Promise.resolve();
         });

    () => cancelled := true;
  });
};

let defaultHeaders =
  Fetch.HeadersInit.makeWithArray([|("Content-Type", "application/json")|]);

let get = (~resource: string) => {
  let requestInit = Fetch.RequestInit.make(~method_=Fetch.Get, ());
  fetchWrapper(~resource, ~requestInit);
};

let post =
    (
      ~resource: string,
      ~headers: Fetch.HeadersInit.t=defaultHeaders,
      ~body: Fetch.bodyInit,
    ) => {
  let requestInit =
    Fetch.RequestInit.make(~method_=Fetch.Post, ~body, ~headers, ());
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

let baseConverter =
    (converter: Fetch.response => Js.Promise.t('kind), result: t)
    : XWonka.Types.sourceT(result('kind)) =>
  switch (result) {
  | Ok(res) =>
    res |> converter |> XWonka.fromPromise |> XWonka.map(value => Ok(value))
  | FailureCode(code) => XWonka.fromValue(FailureCode(code))
  | Failure => XWonka.fromValue(Failure)
  };

let toJson = baseConverter(Fetch.Response.json);
let toText = baseConverter(Fetch.Response.text);
let toArrayBuffer = response =>
  response
  |> baseConverter(Fetch.Response.arrayBuffer)
  |> XWonka.map(result =>
       switch (result) {
       | Ok(value) =>
         let typedArray = Convert.toTypedArray(value);
         Ok(typedArray);
       | FailureCode(status) => FailureCode(status)
       | Failure => Failure
       }
     );