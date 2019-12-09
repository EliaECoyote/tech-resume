// Node crypto bindings
// Further docs: https://nodejs.org/docs/latest-v10.x/api/crypto.html

/**
 * Generates cryptographically strong pseudo-random data.
 * The size argument is a number indicating the number of bytes to generate.
 */
[@bs.module "crypto"]
external randomBytes: int => Node.Buffer.t = "randomBytes";