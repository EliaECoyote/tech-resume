open Jest;
open Expect;

let scheme = "https";
let host = "example.com";
let endpoint = "https://example.com";
let path = "test";
let qsComponents = [|("key1", "value1")|];

describe("buildUrl", () => {
  test("should return correct complete url", () =>
    Url.make(~scheme, ~host, ~path, ~qsComponents, ())
    |> expect
    |> toBe("https://example.com/test?key1=value1")
  );
  test("should return pathname + qs with no scheme", () =>
    Url.make(~host, ~path, ~qsComponents, ())
    |> expect
    |> toBe("/test?key1=value1")
  );
  test("should return pathname + qs with no host", () =>
    Url.make(~scheme, ~path, ~qsComponents, ())
    |> expect
    |> toBe("/test?key1=value1")
  );
  test("should encode qs correctly", () =>
    Url.make(
      ~scheme,
      ~host,
      ~path,
      ~qsComponents=[|("md", "* 1\n* 2\n+-=\n```")|],
      (),
    )
    |> expect
    |> toBe("https://example.com/test?md=*%201%0A*%202%0A%2B-%3D%0A%60%60%60")
  );
  test("should handle endpoint correctly", () =>
    Url.make(~endpoint, ~path, ())
    |> expect
    |> toBe("https://example.com/test")
  );
});