/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */
#pragma once

#include <vros/chain/types.hpp>

namespace vros { namespace chain {

/**
    *  For each action dispatched this receipt is generated
    */
struct action_receipt {
    digest_type act_digest;
    uint64_t    global_sequence = 0;  ///< total number of actions dispatched since genesis

    digest_type
    digest() const { return digest_type::hash(*this); }
};

}}  // namespace vros::chain

FC_REFLECT(vros::chain::action_receipt, (act_digest)(global_sequence))
