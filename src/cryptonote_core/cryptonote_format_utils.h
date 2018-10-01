// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018, The TurtleCoin Developers
//
// Please see the included LICENSE file for more information.

#pragma once

#include <mapbox/variant.hpp>
#include "cryptonote_protocol/blobdatatype.h"
#include "cryptonote_core/cryptonote_basic_impl.h"
#include "crypto/crypto.h"
#include "crypto/hash.h"

using namespace mapbox;

namespace cryptonote
{
  template<typename T>
  bool find_tx_extra_field_by_type(const std::vector<tx_extra_field>& tx_extra_fields, T& field)
  {
    auto it = std::find_if(tx_extra_fields.begin(), tx_extra_fields.end(), [](const tx_extra_field& f) { return typeid(T) == f.type(); });
    if(tx_extra_fields.end() == it)
      return false;

    field = util::get<T>(*it);
    return true;
  }

  bool parse_tx_extra(const std::vector<uint8_t>& tx_extra, std::vector<tx_extra_field>& tx_extra_fields);
  
  bool append_mm_tag_to_extra(std::vector<uint8_t>& tx_extra, const tx_extra_merge_mining_tag& mm_tag);
  bool get_mm_tag_from_extra(const std::vector<uint8_t>& tx_extra, tx_extra_merge_mining_tag& mm_tag);

  void get_blob_hash(const blobdata& blob, crypto::hash& res);
  crypto::hash get_blob_hash(const blobdata& blob);

  crypto::hash get_transaction_hash(const transaction& t);
  bool get_transaction_hash(const transaction& t, crypto::hash& res);
  bool get_transaction_hash(const transaction& t, crypto::hash& res, size_t& blob_size);
  
  bool get_block_hashing_blob(const block& b, blobdata& blob);
  blobdata get_block_hashing_blob(const bb_block& b);
  
  bool get_block_hash(const block& b, crypto::hash& res);
  crypto::hash get_block_hash(const block& b);
  
  bool get_block_header_hash(const block& b, crypto::hash& res);
  bool get_block_longhash(const block& b, crypto::hash& res, uint64_t height);
  crypto::hash get_block_longhash(const block& b, uint64_t height);
  
  bool parse_and_validate_block_from_blob(const blobdata& b_blob, block& b);
  bool parse_and_validate_block_from_blob(const blobdata& b_blob, bb_block& b);

  template<class t_object>
  bool t_serializable_object_to_blob(const t_object& to, blobdata& b_blob)
  {
    std::stringstream ss;
    binary_archive<true> ba(ss);
    bool r = ::serialization::serialize(ba, const_cast<t_object&>(to));
    b_blob = ss.str();
    return r;
  }

  template<class t_object>
  blobdata t_serializable_object_to_blob(const t_object& to)
  {
    blobdata b;
    t_serializable_object_to_blob(to, b);
    return b;
  }

  template<class t_object>
  bool get_object_hash(const t_object& o, crypto::hash& res)
  {
    get_blob_hash(t_serializable_object_to_blob(o), res);
    return true;
  }

  template<class t_object>
  bool get_object_hash(const t_object& o, crypto::hash& res, size_t& blob_size)
  {
    blobdata bl = t_serializable_object_to_blob(o);
    blob_size = bl.size();
    get_blob_hash(bl, res);
    return true;
  }

  blobdata block_to_blob(const block& b);
  bool block_to_blob(const block& b, blobdata& b_blob);
  
  void get_tx_tree_hash(const std::vector<crypto::hash>& tx_hashes, crypto::hash& h);
  crypto::hash get_tx_tree_hash(const std::vector<crypto::hash>& tx_hashes);
  crypto::hash get_tx_tree_hash(const block& b);
  crypto::hash get_tx_tree_hash(const bb_block& b);
}
