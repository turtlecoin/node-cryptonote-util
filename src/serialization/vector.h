// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018, The TurtleCoin Developers
// 
// Please see the included LICENSE file for more information.

#pragma once

#include "serialization.h"

template <template <bool> class Archive, class T>
bool do_serialize(Archive<false> &ar, std::vector<T> &v);
template <template <bool> class Archive, class T>
bool do_serialize(Archive<true> &ar, std::vector<T> &v);

namespace serialization
{
  namespace detail
  {
    template <typename Archive, class T>
    bool serialize_vector_element(Archive& ar, T& e)
    {
      return ::do_serialize(ar, e);
    }

    template <typename Archive>
    bool serialize_vector_element(Archive& ar, uint64_t& e)
    {
      ar.serialize_varint(e);
      return true;
    }
  }
}

template <template <bool> class Archive, class T>
bool do_serialize(Archive<false> &ar, std::vector<T> &v)
{
  size_t cnt;
  ar.begin_array(cnt);
  if (!ar.stream().good())
    return false;
  v.clear();

  // very basic sanity check
  if (ar.remaining_bytes() < cnt) {
    ar.stream().setstate(std::ios::failbit);
    return false;
  }

  v.reserve(cnt);
  for (size_t i = 0; i < cnt; i++) {
    if (i > 0)
      ar.delimit_array();
    v.resize(i+1);
    if (!::serialization::detail::serialize_vector_element(ar, v[i]))
      return false;
    if (!ar.stream().good())
      return false;
  }
  ar.end_array();
  return true;
}

template <template <bool> class Archive, class T>
bool do_serialize(Archive<true> &ar, std::vector<T> &v)
{
  size_t cnt = v.size();
  ar.begin_array(cnt);
  for (size_t i = 0; i < cnt; i++) {
    if (!ar.stream().good())
      return false;
    if (i > 0)
      ar.delimit_array();
    if(!::serialization::detail::serialize_vector_element(ar, v[i]))
      return false;
    if (!ar.stream().good())
      return false;
  }
  ar.end_array();
  return true;
}
