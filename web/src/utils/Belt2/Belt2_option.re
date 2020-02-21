/**
 * nullish coalescing operator for **option** variants
 */
let (<??>) = (a: option('a), b: option('a)) =>
  switch (a) {
  | Some(_) as a => a
  | None => b
  };

/**
 * flatMap with inverted args
 */
let flatMap = (a1, a2) => Belt.Option.flatMap(a2, a1);

/**
 * getWithDefault with inverted args
 */
let getWithDefault = (a1, a2) => Belt.Option.getWithDefault(a2, a1);