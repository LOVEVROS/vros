/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */
#pragma once
#include <vros/chain/types.hpp>
#include <vros/chain/contracts/authorizer_ref.hpp>

namespace vros { namespace chain { namespace contracts {

using meta_key   = name128;
using meta_value = string;

struct meta {
    meta() = default;
    meta(const meta_key& key, const string& value, const authorizer_ref& creator)
        : key(key)
        , value(value)
        , creator(creator) {}

    meta_key       key;
    meta_value     value;
    authorizer_ref creator;
};
using meta_list = std::vector<meta>;

}}}  // namespac vros::chain::contracts

FC_REFLECT(vros::chain::contracts::meta, (key)(value)(creator))
