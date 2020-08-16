// Copyright (c) 2020, The TurtleCoin Developers
//
// Please see the included LICENSE file for more information.

/** ignore */
const native = require('bindings')('cryptonote.node');

export function construct_block_blob(template: Buffer, nonce: Buffer): Buffer {
    return native.construct_block_blob(template, nonce);
}

export function get_block_id(blob: Buffer): Buffer {
    return native.get_block_id(blob);
}

export function convert_blob(blob: Buffer): Buffer {
    return native.convert_blob(blob)
}

export function convert_blob_bb(blob: Buffer): Buffer {
    return native.convert_blob_bb(blob);
}

/**
 * Calculates the address prefix in decimal form from the given address
 * @param address the public wallet address to decode
 */
export function address_decode (address: Buffer): number {
    return native.address_decode(address);
}
