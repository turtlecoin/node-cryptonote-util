// Copyright (c) 2020, The TurtleCoin Developers
//
// Please see the included LICENSE file for more information.

import CryptoNoteUtils from '../typescript';
import {describe, it} from 'mocha';
import * as assert from 'assert';

const block = {
    blocktemplate: '0500b4318c1249164393f7b9d691e60aba81ca9bbffb9e0b23e6b01c93d9c621ab800000b5f9abe60500000000000' +
        '0000000000000000000000000000000000000000000000000000000000000010000000023032100000000000000000000000000000' +
        '000000000000000000000000000000000000001b9fe5801ff91fe58070802959f9d01c4cbd664e7c937687e7ae847f75745614ba4a' +
        '11a6f5c0b25a5ca42c314026840ba5dc6c84a533d005f0a6f9758c4eb4d19bffa3b4a73547596dac17b7d169003022f88b7b85f42c' +
        '789721e7ee44bded4b6e1e48f7fb9bb73dc87cda264adf06a54f02e02175cea7cf09b914749c650b9321678d3f0e4390cad20ac689' +
        '6be7ac14ba4f31ab0ea01021192fdc86fd815642850c0f50bc11df8ba1d39581d2c0642aa83f72f1c3fb22580ea3002c28a03fb9b3' +
        'b7f6e4f9cdf2e74e93c8282580d8134722302b9978cdc3a35f39980897a02a4978ee1742da6f77791f065326922ff8c713687d2b52' +
        '215ce7911b08922a7602901296929051bf0b258b717ffdca80c8b0822063c9d3f70f0dbed9b3f2e0aca2fa1020600000000000012f' +
        '6b48f1800c368e046252e50165620fbb155176d17ddfdb98fd3227237760cf579af90593ec64a4a5549bd219969dc9aa1aac2dc3ea' +
        'b529c90b2f8c20221f99f02bdfb9a7efb5e5e85b710ef442b3cc82431173db142947a4f755a22f45915e5f2d53027a4aee505305c7' +
        'cde822f960578417779ef71c1c8b611e95f1643fd125a39df66f40ce82794c9debc342381f26763c80968f8bf8e5378b433c583a60' +
        '78348457ebe6ad61e5487ac3bebe22e03c46d9c894a511561ffbe9612809c17207493941d607b152f4ce193f828085ee8afcc0a351' +
        'a9cecac0f01b2729df67af831637f76a8430848d86beb643df918af6cc554a8d0b42899671ee02cfbe64905b4458647a90a50aa307' +
        'be48f3db57ff7f5446926ee70cee0c0cf7ad9021de727b2f97b5e2f0fd0103b24725d0c11ddd876de27a7b837bdd43eb4d9ca47988' +
        '3ad2dcd853ece92e6a7544b24e44051411e6b913760b9d997ca986985b92d51ccbc5708f0cd93c0ce2d1aea46a6744cc81a3df394c' +
        'd099518cb3c3a5fa43e187c9d72bc3a245256fa60b450411b56d2258d4001b6133f58a5d48af0cc12af652c202b3c9de3f42602650' +
        '0c493c075340ca713a49843db16bda542fd76fb597914e1c975a70d12971baf70b8f7f6f452fd669f3cc00d0a0f36a3312f7c51cad' +
        'f24cac9c24841eeaa742b5d1e74a750e5763afbd2a75e341edb4439801ac665dd0a3b16cc648a651546c7a3633f0a0274bb3815d6b' +
        'da05e12fb7cbb908ceb582590a84a30ba6043178279343f5107713390b8e2ca2a8723ee7e5855c6af674bbd9b5a',
    difficulty: 9796275295,
    height: 1457937,
    reservedOffset: 411
};

const expected_mining_blob = '0100b5f9abe605b4318c1249164393f7b9d691e60aba81ca9bbffb9e0b23e6b01c93d9c621ab800000000' +
    '04b27c162bc89b0bdfa0db8b5c99977943caf754bb6181d8e1bafc6af2ab0b0bb01';

const expected_merged_block = '0500b4318c1249164393f7b9d691e60aba81ca9bbffb9e0b23e6b01c93d9c621ab800100b5f9abe605b4' +
    '318c1249164393f7b9d691e60aba81ca9bbffb9e0b23e6b01c93d9c621ab80641c00000101000000230321008a7f7239a53ead2db2fc10' +
    '62a898c1af301e919e2f26e13a568176ba7f1a0e1f01b9fe5801ff91fe58070802959f9d01c4cbd664e7c937687e7ae847f75745614ba4' +
    'a11a6f5c0b25a5ca42c314026840ba5dc6c84a533d005f0a6f9758c4eb4d19bffa3b4a73547596dac17b7d169003022f88b7b85f42c789' +
    '721e7ee44bded4b6e1e48f7fb9bb73dc87cda264adf06a54f02e02175cea7cf09b914749c650b9321678d3f0e4390cad20ac6896be7ac1' +
    '4ba4f31ab0ea01021192fdc86fd815642850c0f50bc11df8ba1d39581d2c0642aa83f72f1c3fb22580ea3002c28a03fb9b3b7f6e4f9cdf' +
    '2e74e93c8282580d8134722302b9978cdc3a35f39980897a02a4978ee1742da6f77791f065326922ff8c713687d2b52215ce7911b08922' +
    'a7602901296929051bf0b258b717ffdca80c8b0822063c9d3f70f0dbed9b3f2e0aca2fa1020600000000000012f6b48f1800c368e04625' +
    '2e50165620fbb155176d17ddfdb98fd3227237760cf579af90593ec64a4a5549bd219969dc9aa1aac2dc3eab529c90b2f8c20221f99f02' +
    'bdfb9a7efb5e5e85b710ef442b3cc82431173db142947a4f755a22f45915e5f2d53027a4aee505305c7cde822f960578417779ef71c1c8' +
    'b611e95f1643fd125a39df66f40ce82794c9debc342381f26763c80968f8bf8e5378b433c583a6078348457ebe6ad61e5487ac3bebe22e' +
    '03c46d9c894a511561ffbe9612809c17207493941d607b152f4ce193f828085ee8afcc0a351a9cecac0f01b2729df67af831637f76a843' +
    '0848d86beb643df918af6cc554a8d0b42899671ee02cfbe64905b4458647a90a50aa307be48f3db57ff7f5446926ee70cee0c0cf7ad902' +
    '1de727b2f97b5e2f0fd0103b24725d0c11ddd876de27a7b837bdd43eb4d9ca479883ad2dcd853ece92e6a7544b24e44051411e6b913760' +
    'b9d997ca986985b92d51ccbc5708f0cd93c0ce2d1aea46a6744cc81a3df394cd099518cb3c3a5fa43e187c9d72bc3a245256fa60b45041' +
    '1b56d2258d4001b6133f58a5d48af0cc12af652c202b3c9de3f426026500c493c075340ca713a49843db16bda542fd76fb597914e1c975' +
    'a70d12971baf70b8f7f6f452fd669f3cc00d0a0f36a3312f7c51cadf24cac9c24841eeaa742b5d1e74a750e5763afbd2a75e341edb4439' +
    '801ac665dd0a3b16cc648a651546c7a3633f0a0274bb3815d6bda05e12fb7cbb908ceb582590a84a30ba6043178279343f5107713390b8' +
    'e2ca2a8723ee7e5855c6af674bbd9b5a'

const expected_block_id = '8e466960ef1cfffdcac94f8b0595d9edbcd54559649a1bfc934141f9ab013e9a';

describe('CryptoNote Utilities Tests', async () => {
    it('Address Prefix Decoding', async () => {
        const address = Buffer.from(
            'TRTLuxN6FVALYxeAEKhtWDYNS9Vd9dHVp3QHwjKbo76ggQKgUfVjQp8iPypECCy3MwZVyu89k1fWE2Ji6EKedbrqECHHWouZN6g');

        const expected_prefix = 3914525;

        const prefix = await CryptoNoteUtils.address_decode(address);

        assert(prefix === expected_prefix);
    })

    it('To Mining Blob', async () => {
        const blob = Buffer.from(block.blocktemplate, 'hex');

        const miningBlob = await CryptoNoteUtils.convert_blob(blob);

        assert(miningBlob.toString('hex') === expected_mining_blob);
    })

    it('Merge Blocks: Nonce Number', async () => {
        const blob = Buffer.from(block.blocktemplate, 'hex');

        const merged = await CryptoNoteUtils.construct_block_blob(blob, 0x1c64);

        assert(merged.toString('hex') === expected_merged_block);
    })

    it('Merge Blocks: Nonce Buffer', async () => {
        const blob = Buffer.from(block.blocktemplate, 'hex');

        const nonce = Buffer.alloc(4);

        nonce.writeUInt32LE(0x1c64, 0);

        const merged = await CryptoNoteUtils.construct_block_blob(blob, nonce);

        assert(merged.toString('hex') === expected_merged_block);
    })

    it('Calculates Block ID', async () => {
        const block = Buffer.from(expected_merged_block, 'hex');

        const id = await CryptoNoteUtils.get_block_id(block);

        assert(id.toString('hex') === expected_block_id);
    })
})