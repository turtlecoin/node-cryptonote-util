// Copyright (c) 2020, The TurtleCoin Developers
//
// Please see the included LICENSE file for more information.

import {address_decode} from "../typescript";
import {describe, it} from 'mocha';
import * as assert from 'assert';

describe('CryptoNote Utilities Tests', () => {
    it('Address Prefix Decoding', () => {
        const address = Buffer.from('TRTLuxN6FVALYxeAEKhtWDYNS9Vd9dHVp3QHwjKbo76ggQKgUfVjQp8iPypECCy3MwZVyu89k1fWE2Ji6EKedbrqECHHWouZN6g');

        const expected_prefix = 3914525;

        const prefix = address_decode(address);

        assert(prefix === expected_prefix);
    })
})