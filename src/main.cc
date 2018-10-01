// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018, The TurtleCoin Developers
// 
// Please see the included LICENSE file for more information.

#include <cmath>
#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <stdint.h>
#include <string>
#include <algorithm>
#include "cryptonote_core/cryptonote_basic.h"
#include "cryptonote_core/cryptonote_format_utils.h"
#include "cryptonote_protocol/blobdatatype.h"
#include "crypto/crypto.h"
#include "crypto/hash.h"
#include "common/base58.h"
#include "serialization/binary_utils.h"
#include <nan.h>

namespace cryptonoteutil {
using namespace node;
using namespace v8;
using namespace cryptonote;

void THROW_ERROR_EXCEPTION(const char* msg) {
    return Nan::ThrowError(msg);
}

blobdata uint64be_to_blob(uint64_t num) {
    blobdata res = "        ";
    res[0] = num >> 56 & 0xff;
    res[1] = num >> 48 & 0xff;
    res[2] = num >> 40 & 0xff;
    res[3] = num >> 32 & 0xff;
    res[4] = num >> 24 & 0xff;
    res[5] = num >> 16 & 0xff;
    res[6] = num >> 8  & 0xff;
    res[7] = num       & 0xff;
    return res;
}


static bool fillExtra(cryptonote::block& block1, const cryptonote::block& block2) {
    // (src/cryptonote_core/cryptonote_basic)
    cryptonote::tx_extra_merge_mining_tag mm_tag;
    mm_tag.depth = 0;
    // (src/cryptonote_core/cryptonote_format_utils)
    if (!cryptonote::get_block_header_hash(block2, mm_tag.merkle_root))
        return false;

    block1.miner_tx.extra.clear();
    // (src/cryptonote_core/cryptonote_format_utils)
    if (!cryptonote::append_mm_tag_to_extra(block1.miner_tx.extra, mm_tag))
        return false;

    return true;
}

static bool mergeBlocks(const cryptonote::block& block1, cryptonote::block& block2, const std::vector<crypto::hash>& branch2) {
    block2.timestamp = block1.timestamp;
    block2.parent_block.major_version = block1.major_version;
    block2.parent_block.minor_version = block1.minor_version;
    block2.parent_block.prev_id = block1.prev_id;
    block2.parent_block.nonce = block1.nonce;
    block2.parent_block.miner_tx = block1.miner_tx;
    block2.parent_block.number_of_transactions = block1.tx_hashes.size() + 1;
    // (src/crypto/tree-hash)
    block2.parent_block.miner_tx_branch.resize(crypto::tree_depth(block1.tx_hashes.size() + 1));
    std::vector<crypto::hash> transactionHashes;
    // (src/cryptonote_core/cryptonote_format_utils)
    transactionHashes.push_back(cryptonote::get_transaction_hash(block1.miner_tx));
    std::copy(block1.tx_hashes.begin(), block1.tx_hashes.end(), std::back_inserter(transactionHashes));
    // (src/crypto/tree-hash)
    tree_branch(transactionHashes.data(), transactionHashes.size(), block2.parent_block.miner_tx_branch.data());
    block2.parent_block.blockchain_branch = branch2;
    return true;
}

static bool construct_parent_block(const cryptonote::block& b, cryptonote::block& parent_block) {
    parent_block.major_version = 1;
    parent_block.minor_version = 0;
    parent_block.timestamp = b.timestamp;
    parent_block.prev_id = b.prev_id;
    parent_block.nonce = b.parent_block.nonce;
    parent_block.miner_tx.version = CURRENT_TRANSACTION_VERSION;
    parent_block.miner_tx.unlock_time = 0;

    return fillExtra(parent_block, b);
}

NAN_METHOD(convert_blob) {

    if (info.Length() < 1)
        return THROW_ERROR_EXCEPTION("You must provide one argument.");

    Local<Object> target = info[0]->ToObject();

    if (!Buffer::HasInstance(target))
        return THROW_ERROR_EXCEPTION("Argument should be a buffer object.");

    blobdata input = std::string(Buffer::Data(target), Buffer::Length(target));
    blobdata output = "";

    //convert
    block b = block();
    if (!parse_and_validate_block_from_blob(input, b))
        return THROW_ERROR_EXCEPTION("Failed to parse block");

    if (b.major_version < BLOCK_MAJOR_VERSION_2) {
        // (src/cryptonote_core/cryptonote_format_utils)
        if (!get_block_hashing_blob(b, output))
            return THROW_ERROR_EXCEPTION("Failed to create mining block");
    } else {
        block parent_block;
        if (!construct_parent_block(b, parent_block))
            return THROW_ERROR_EXCEPTION("Failed to construct parent block");

        // (src/cryptonote_core/cryptonote_format_utils)
        if (!get_block_hashing_blob(parent_block, output))
            return THROW_ERROR_EXCEPTION("Failed to create mining block");
    }

    v8::Local<v8::Value> returnValue = Nan::CopyBuffer((char*)output.data(), output.size()).ToLocalChecked();
    info.GetReturnValue().Set(returnValue);
}


NAN_METHOD(get_block_id) {

    if (info.Length() < 1)
        return THROW_ERROR_EXCEPTION("You must provide one argument.");

    Local<Object> target = info[0]->ToObject();

    if (!Buffer::HasInstance(target))
        return THROW_ERROR_EXCEPTION("Argument should be a buffer object.");

    blobdata input = std::string(Buffer::Data(target), Buffer::Length(target));
    blobdata output = "";

    block b = block();
    if (!parse_and_validate_block_from_blob(input, b))
        return THROW_ERROR_EXCEPTION("Failed to parse block");

    crypto::hash block_id;
    // (src/cryptonote_core/cryptonote_format_utils)
    if (!get_block_hash(b, block_id))
        return THROW_ERROR_EXCEPTION("Failed to calculate hash for block");

    char *cstr = reinterpret_cast<char*>(&block_id);
    v8::Local<v8::Value> returnValue = Nan::CopyBuffer(cstr, 32).ToLocalChecked();
    info.GetReturnValue().Set(returnValue);
}

NAN_METHOD(construct_block_blob) {

    if (info.Length() < 2)
        return THROW_ERROR_EXCEPTION("You must provide two arguments.");

    Local<Object> block_template_buf = info[0]->ToObject();
    Local<Object> nonce_buf = info[1]->ToObject();

    if (!Buffer::HasInstance(block_template_buf) || !Buffer::HasInstance(nonce_buf))
        return THROW_ERROR_EXCEPTION("Both arguments should be buffer objects.");

    if (Buffer::Length(nonce_buf) != 4)
        return THROW_ERROR_EXCEPTION("Nonce buffer has invalid size.");

    uint32_t nonce = *reinterpret_cast<uint32_t*>(Buffer::Data(nonce_buf));

    blobdata block_template_blob = std::string(Buffer::Data(block_template_buf), Buffer::Length(block_template_buf));
    blobdata output = "";

    block b = block();
    // (src/cryptonote_core/cryptonote_format_utils)
    if (!parse_and_validate_block_from_blob(block_template_blob, b))
        return THROW_ERROR_EXCEPTION("Failed to parse block");

    b.nonce = nonce;
    if (b.major_version == BLOCK_MAJOR_VERSION_2) {
        block parent_block;
        b.parent_block.nonce = nonce;
        if (!construct_parent_block(b, parent_block))
            return THROW_ERROR_EXCEPTION("Failed to construct parent block");

        if (!mergeBlocks(parent_block, b, std::vector<crypto::hash>()))
            return THROW_ERROR_EXCEPTION("Failed to postprocess mining block");
    }
    if (b.major_version == BLOCK_MAJOR_VERSION_3) {
        block parent_block;
        b.parent_block.nonce = nonce;
        if (!construct_parent_block(b, parent_block))
            return THROW_ERROR_EXCEPTION("Failed to construct parent block");

        if (!mergeBlocks(parent_block, b, std::vector<crypto::hash>()))
            return THROW_ERROR_EXCEPTION("Failed to postprocess mining block");
    }
    if (b.major_version == BLOCK_MAJOR_VERSION_4) {
      block parent_block;
      b.parent_block.nonce = nonce;
      if (!construct_parent_block(b, parent_block))
        return THROW_ERROR_EXCEPTION("Failed to construct parent block");

      if (!mergeBlocks(parent_block, b, std::vector<crypto::hash>()))
        return THROW_ERROR_EXCEPTION("Failed to postprocess mining block");
    }

    // (src/cryptonote_core/cryptonote_format_utils)
    if (!block_to_blob(b, output))
        return THROW_ERROR_EXCEPTION("Failed to convert block to blob");

    v8::Local<v8::Value> returnValue = Nan::CopyBuffer((char*)output.data(), output.size()).ToLocalChecked();
    info.GetReturnValue().Set(returnValue);
}

NAN_METHOD(convert_blob_bb) {

    if (info.Length() < 1)
        return THROW_ERROR_EXCEPTION("You must provide one argument.");

    Local<Object> target = info[0]->ToObject();

    if (!Buffer::HasInstance(target))
        return THROW_ERROR_EXCEPTION("Argument should be a buffer object.");

    blobdata input = std::string(Buffer::Data(target), Buffer::Length(target));
    blobdata output = "";

    //convert
    bb_block b = bb_block();
    // (src/cryptonote_core/cryptonote_format_utils)
    if (!parse_and_validate_block_from_blob(input, b)) {
        return THROW_ERROR_EXCEPTION("Failed to parse block");
    }
    // (src/cryptonote_core/cryptonote_format_utils)
    output = get_block_hashing_blob(b);

    v8::Local<v8::Value> returnValue = Nan::CopyBuffer((char*)output.data(), output.size()).ToLocalChecked();
    info.GetReturnValue().Set(returnValue);
}

NAN_METHOD(address_decode) {

    if (info.Length() < 1)
        return THROW_ERROR_EXCEPTION("You must provide one argument.");

    Local<Object> target = info[0]->ToObject();

    if (!Buffer::HasInstance(target))
        return THROW_ERROR_EXCEPTION("Argument should be a buffer object.");

    blobdata input = std::string(Buffer::Data(target), Buffer::Length(target));

    blobdata data;
    uint64_t prefix;
    // (src/common/base58)
    if (!tools::base58::decode_addr(input, prefix, data)) {
        info.GetReturnValue().Set(Nan::Undefined());
    }

    account_public_address adr;
    // (src/serialization/binary_utils)
    if (!::serialization::parse_binary(data, adr)) {
        info.GetReturnValue().Set(Nan::Undefined());
    }

    // (src/crypto/crypto)
    if (!crypto::check_key(adr.m_spend_public_key) || !crypto::check_key(adr.m_view_public_key)) {
        info.GetReturnValue().Set(Nan::Undefined());
    }

    info.GetReturnValue().Set(Nan::New(static_cast<uint32_t>(prefix)));
}

NAN_MODULE_INIT(Init) {
    Nan::Set(target, Nan::New("construct_block_blob").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(construct_block_blob)).ToLocalChecked());
    Nan::Set(target, Nan::New("get_block_id").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(get_block_id)).ToLocalChecked());
    Nan::Set(target, Nan::New("convert_blob").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(convert_blob)).ToLocalChecked());
    Nan::Set(target, Nan::New("convert_blob_bb").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(convert_blob_bb)).ToLocalChecked());
    Nan::Set(target, Nan::New("address_decode").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(address_decode)).ToLocalChecked());
}

NODE_MODULE(cryptonote, Init)
}
