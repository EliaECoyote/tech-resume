/**
 * flatMap with inverted args
 */
let flatMap = (a1, a2) => Belt.Result.flatMap(a2, a1);

/**
 * getWithDefault with inverted args
 */
let getWithDefault = (a1, a2) => Belt.Result.getWithDefault(a2, a1);