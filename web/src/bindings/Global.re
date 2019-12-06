// TODO: test this bindings!

/**
 * The encodeURIComponent() function encodes a Uniform Resource
 * Identifier (URI) component by replacing each instance of certain
 * characters by one, two, three, or four escape sequences representing
 * the UTF-8 encoding of the character (will only be four escape
 * sequences for characters composed of two "surrogate" characters).
 * Escapes all characters except:
 *     A-Z a-z 0-9 - _ . ! ~ * ' ( )
 * [further documentation](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/encodeURIComponent)
 */
[@bs.val]
external encodeURIComponent: string => string = "encodeURIComponent";

/**
 * The encodeURI() function encodes a Uniform Resource Identifier (URI)
 * by replacing each instance of certain characters by one, two, three,
 * or four escape sequences representing the UTF-8 encoding of the
 * character (will only be four escape sequences for characters
 * composed of two "surrogate" characters).
 * Escapes all characters except:
 *     A-Z a-z 0-9 ; , / ? : @ & = + $ - _ . ! ~ * ' ( ) #
 * [further documentation](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/encodeURI)
 */
[@bs.val]
external encodeURI: string => string = "encodeURI";

type base64;

/**
 * creates a base-64 encoded ASCII string from a binary string
 * (i.e., a String object in which each character in the string is
 * treated as a byte of binary data).
 * [further documentation](https://developer.mozilla.org/en-US/docs/Web/API/WindowOrWorkerGlobalScope/btoa)
 */
[@bs.val]
external stringToBase64: string => base64 = "btoa";
let stringToBase64 = str => {
  let result = stringToBase64(str);
  switch (result) {
  | exception (Js.Exn.Error(err)) =>
    Js.Exn.message(err)
    |> Js.Option.getWithDefault("string to base64 conversion error")
    |> (msg => Belt.Result.Error(msg))
  | exception _ => Belt.Result.Error("string to base64 conversion error")
  | value => Belt.Result.Ok(value)
  };
};

/**
 * decodes a string of data which has been encoded using base-64
 * encoding. You can use the btoa() method to encode and transmit
 * data which may otherwise cause communication problems, then
 * transmit it and use the atob() method to decode the data again.
 * For example, you can encode, transmit, and decode control
 * characters such as ASCII values 0 through 31.
 * [further documentation](https://developer.mozilla.org/en-US/docs/Web/API/WindowOrWorkerGlobalScope/atob)
 */
[@bs.val]
external base64ToString: base64 => string = "atob";
let base64ToString = b64 => {
  let result = base64ToString(b64);
  switch (result) {
  | exception (Js.Exn.Error(err)) =>
    Js.Exn.message(err)
    |> Js.Option.getWithDefault("base64 to string conversion error")
    |> (msg => Error(msg))
  | exception _ => Error("base64 to string conversion error")
  | value => Ok(value)
  };
};