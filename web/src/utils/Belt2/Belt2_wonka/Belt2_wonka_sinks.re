include Wonka_sinks;

/**
 * uncurried subscribe
 */
let subscribe = callback => Wonka.subscribe((. value) => callback(value));