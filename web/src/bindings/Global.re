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