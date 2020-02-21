/**
 * nullish coalescing operator for **option** variants
 */
let (<??>) = (a: option('a), b: option('a)) =>
  switch (a) {
  | Some(_) as a => a
  | None => b
  };