// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018, The TurtleCoin Developers
//
// Please see the included LICENSE file for more information.

#include <algorithm>
#include <assert.h>

#include "cryptonote_format_utils.h"
#include "cryptonote_config.h"
#include "crypto/crypto.h"
#include "crypto/hash.h"
#include "serialization/binary_utils.h"

namespace cryptonote
{
  uint64_t power_integral(uint64_t a, uint64_t b)
  {
    if(b == 0)
      return 1;
    uint64_t total = a;
    for(uint64_t i = 1; i != b; i++)
      total *= a;
    return total;
  }

  bool parse_tx_extra(const std::vector<uint8_t>& tx_extra, std::vector<tx_extra_field>& tx_extra_fields)
  {
    tx_extra_fields.clear();

    if(tx_extra.empty())
      return true;

    std::string extra_str(reinterpret_cast<const char*>(tx_extra.data()), tx_extra.size());
    std::istringstream iss(extra_str);
    binary_archive<false> ar(iss);

    bool eof = false;
    while (!eof)
    {
      tx_extra_field field;
      bool r = ::do_serialize(ar, field);
      assert(r);
      tx_extra_fields.push_back(field);

      std::ios_base::iostate state = iss.rdstate();
      eof = (EOF == iss.peek());
      iss.clear(state);
    }
    assert(::serialization::check_stream_state(ar));

    return true;
  }

  bool append_mm_tag_to_extra(std::vector<uint8_t>& tx_extra, const tx_extra_merge_mining_tag& mm_tag)
  {
    blobdata blob;
    if (!t_serializable_object_to_blob(mm_tag, blob))
      return false;

    tx_extra.push_back(TX_EXTRA_MERGE_MINING_TAG);
    std::copy(reinterpret_cast<const uint8_t*>(blob.data()), reinterpret_cast<const uint8_t*>(blob.data() + blob.size()), std::back_inserter(tx_extra));
    return true;
  }

  bool get_mm_tag_from_extra(const std::vector<uint8_t>& tx_extra, tx_extra_merge_mining_tag& mm_tag)
  {
    std::vector<tx_extra_field> tx_extra_fields;
    if (!parse_tx_extra(tx_extra, tx_extra_fields))
      return false;

    return find_tx_extra_field_by_type(tx_extra_fields, mm_tag);
  }

  void get_blob_hash(const blobdata& blob, crypto::hash& res)
  {
    cn_fast_hash(blob.data(), blob.size(), res);
  }

  crypto::hash get_blob_hash(const blobdata& blob)
  {
    crypto::hash h = null_hash;
    get_blob_hash(blob, h);
    return h;
  }

  crypto::hash get_transaction_hash(const transaction& t)
  {
    crypto::hash h = null_hash;
    size_t blob_size = 0;
    get_object_hash(t, h, blob_size);
    return h;
  }

  bool get_transaction_hash(const transaction& t, crypto::hash& res)
  {
    size_t blob_size = 0;
    return get_object_hash(t, res, blob_size);
  }

  bool get_transaction_hash(const bb_transaction& t, crypto::hash& res)
  {
    size_t blob_size = 0;
    return get_object_hash(static_cast<const bb_transaction_prefix&>(t), res, blob_size);
  }

  bool get_transaction_hash(const transaction& t, crypto::hash& res, size_t& blob_size)
  {
    return get_object_hash(t, res, blob_size);
  }

  bool get_block_hashing_blob(const block& b, blobdata& blob)
  {
    blob = t_serializable_object_to_blob(static_cast<const block_header&>(b));
    crypto::hash tree_root_hash = get_tx_tree_hash(b);
    blob.append(reinterpret_cast<const char*>(&tree_root_hash), sizeof(tree_root_hash));
    blob.append(tools::get_varint_data(b.tx_hashes.size()+1));

    return true;
  }

  blobdata get_block_hashing_blob(const bb_block& b)
  {
    blobdata blob = t_serializable_object_to_blob(static_cast<bb_block_header>(b));
    crypto::hash tree_root_hash = get_tx_tree_hash(b);
    blob.append((const char*)&tree_root_hash, sizeof(tree_root_hash ));
    blob.append(tools::get_varint_data(b.tx_hashes.size()+1));
    return blob;
  }

  bool get_block_hash(const block& b, crypto::hash& res)
  {
    blobdata blob;
    if (!get_block_hashing_blob(b, blob))
      return false;

    if (BLOCK_MAJOR_VERSION_2 <= b.major_version)
    {
      blobdata parent_blob;
      auto sbb = make_serializable_bytecoin_block(b, true, false);
      if (!t_serializable_object_to_blob(sbb, parent_blob))
        return false;

      blob.append(parent_blob);
    }

    return get_object_hash(blob, res);
  }

  crypto::hash get_block_hash(const block& b)
  {
    crypto::hash p = null_hash;
    get_block_hash(b, p);
    return p;
  }

  bool get_block_header_hash(const block& b, crypto::hash& res)
  {
    blobdata blob;
    if (!get_block_hashing_blob(b, blob))
      return false;

    return get_object_hash(blob, res);
  }

  bool get_block_longhash(const block& b, crypto::hash& res, uint64_t height)
  {
    blobdata bd;
    if(!get_block_hashing_blob(b, bd))
      return false;
    crypto::cn_slow_hash(bd.data(), bd.size(), res);
    return true;
  }

  crypto::hash get_block_longhash(const block& b, uint64_t height)
  {
    crypto::hash p = null_hash;
    get_block_longhash(b, p, height);
    return p;
  }

  bool parse_and_validate_block_from_blob(const blobdata& b_blob, block& b)
  {
    std::stringstream ss;
    ss << b_blob;
    binary_archive<false> ba(ss);
    bool r = ::serialization::serialize(ba, b);
    assert(r);
    return true;
  }
  bool parse_and_validate_block_from_blob(const blobdata& b_blob, bb_block& b)
  {
    std::stringstream ss;
    ss << b_blob;
    binary_archive<false> ba(ss);
    bool r = ::serialization::serialize(ba, b);
    assert(r);
    return true;
  }

  blobdata block_to_blob(const block& b)
  {
    return t_serializable_object_to_blob(b);
  }

  bool block_to_blob(const block& b, blobdata& b_blob)
  {
    return t_serializable_object_to_blob(b, b_blob);
  }

  void get_tx_tree_hash(const std::vector<crypto::hash>& tx_hashes, crypto::hash& h)
  {
    tree_hash(tx_hashes.data(), tx_hashes.size(), h);
  }

  crypto::hash get_tx_tree_hash(const std::vector<crypto::hash>& tx_hashes)
  {
    crypto::hash h = null_hash;
    get_tx_tree_hash(tx_hashes, h);
    return h;
  }

  crypto::hash get_tx_tree_hash(const block& b)
  {
    std::vector<crypto::hash> txs_ids;
    crypto::hash h = null_hash;
    size_t bl_sz = 0;
    get_transaction_hash(b.miner_tx, h, bl_sz);
    txs_ids.push_back(h);
    for (const auto &tx : b.tx_hashes)
    {
      txs_ids.push_back(tx);
    }
    return get_tx_tree_hash(txs_ids);
  }

  crypto::hash get_tx_tree_hash(const bb_block& b)
  {
    std::vector<crypto::hash> txs_ids;
    crypto::hash h = null_hash;
    get_transaction_hash(b.miner_tx, h);
    txs_ids.push_back(h);
    for (const auto &tx : b.tx_hashes)
    {
      txs_ids.push_back(tx);
    }
    return get_tx_tree_hash(txs_ids);
  }

}
