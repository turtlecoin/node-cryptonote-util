// Copyright (c) 2020, The TurtleCoin Developers
//
// Please see the included LICENSE file for more information.

/** @ignore */
const native = require('bindings')('cryptonote.node');

/**
 * Calculates the address prefix in decimal form from the given address
 * @param address the public wallet address to decode
 */
export function address_decode (address: Buffer): number {
    return native.address_decode(address);
}

/**
 * Constructs a new "final" block that can be submitted to the network
 * from the original block in the block template and the nonce value
 * found by the miner(s)
 * @param block the block template blob
 * @param nonce
 */
export function construct_block_blob(block: Buffer, nonce: Buffer | number): Buffer {
    if (!(nonce instanceof Buffer)) {
        const tmp = Buffer.alloc(4);

        tmp.writeUInt32LE(nonce, 0);

        nonce = tmp;
    }

    return native.construct_block_blob(block, nonce);
}

/**
 * Converts a block into a v1 hashing block typically used by miners during
 * mining operations. This method actually creates a merged mining block
 * that merge mines itself
 * @param template the block template blob
 */
export function convert_blob(template: Buffer): Buffer {
    return native.convert_blob(template)
}

/**
 * Legacy convert block (prior to merged mining) that parses and validates
 * a block blob from a block template
 * @param template the block template blob
 */
export function convert_blob_bb(template: Buffer): Buffer {
    return native.convert_blob_bb(template);
}

/**
 * Calculates the block id (hash) of the given blob
 * @param blob the block blob
 */
export function get_block_id(blob: Buffer): Buffer {
    return native.get_block_id(blob);
}

/**
 * Provides CryptoNote based utilities for block manipulation using a
 * native Node.js c++ addon. The methods in this class are provided
 * as async primitives that wrap the synchronous versions for those
 * that wish to use Typescript
 */
export default class CryptoNoteUtils {
    /**
     * Calculates the address prefix in decimal form from the given address
     * @param address the public wallet address to decode
     */
    public static async address_decode(address: Buffer): Promise<number> {
        const l_address = Buffer.from(address);

        return address_decode(l_address);
    }

    /**
     * Constructs a new "final" block that can be submitted to the network
     * from the original block in the block template and the nonce value
     * found by the miner(s)
     * @param block the block blob
     * @param nonce
     */
    public static async construct_block_blob(block: Buffer, nonce: Buffer | number): Promise<Buffer> {
        return construct_block_blob(block, nonce);
    }

    /**
     * Converts a block into a v1 hashing block typically used by miners during
     * mining operations. This method actually creates a merged mining block
     * that merge mines itself
     * @param template the block template blob
     */
    public static async convert_blob(template: Buffer): Promise<Buffer> {
        return convert_blob(template);
    }

    /**
     * Legacy convert block (prior to merged mining) that parses and validates
     * a block blob from a block template
     * @param template the block template blob
     */
    public static async convert_blob_bb(template: Buffer): Promise<Buffer> {
        return convert_blob_bb(template);
    }

    /**
     * Calculates the block id (hash) of the given blob
     * @param blob the block blob
     */
    public static async get_block_id(blob: Buffer): Promise<Buffer> {
        return get_block_id(blob);
    }
}