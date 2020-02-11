let a = 5;

let encode = ((key: string, value: string)) =>
  key ++ "=" ++ Js.Global.encodeURIComponent(value);
let joinWithAmpersand = Js.Array.joinWith("&");
let addInitialQuestionMark = value => {j|?$value|j};

/**
 * builds an url from url components. Special characters gets automatically escaped.
 * The output url won't contain both scheme and host when one of them is not provided
 */
let make =
    (
      ~scheme: option(string)=?,
      ~host: option(string)=?,
      ~endpoint: string="",
      ~path: string="",
      ~qsComponents: option(array((string, string)))=?,
      (),
    ) => {
  let endpoint =
    switch (scheme, host) {
    | (Some(schemeValue), Some(hostValue)) => {j|$schemeValue://$hostValue|j}
    | _ => endpoint
    };
  let qs =
    switch (qsComponents) {
    | Some(value) =>
      value
      |> Array.map(encode)
      |> joinWithAmpersand
      |> addInitialQuestionMark
    | None => ""
    };
  {j|$endpoint/$path$qs|j};
};