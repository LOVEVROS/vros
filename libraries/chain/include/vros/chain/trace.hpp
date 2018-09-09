/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */
#pragma once

#include <vros/chain/action.hpp>
#include <vros/chain/action_receipt.hpp>
#include <vros/chain/block.hpp>

namespace vros { namespace chain {

struct action_trace {
    action_trace(const action_receipt& r)
        : receipt(r) {}
    action_trace() {}

    action_receipt   receipt;
    action           act;
    fc::microseconds elapsed;
    string           console;

    transaction_id_type trx_id;
};

struct transaction_trace;
using transaction_trace_ptr = std::shared_ptr<transaction_trace>;

struct transaction_trace {
    transaction_id_type                      id;
    fc::optional<transaction_receipt_header> receipt;
    fc::microseconds                         elapsed;
    bool                                     is_suspend = false;
    vector<action_trace>                     action_traces;  ///< disposable

    uint32_t charge;

    fc::optional<fc::exception> except;
    std::exception_ptr          except_ptr;
};

struct block_trace {
    fc::microseconds              elapsed;
    vector<transaction_trace_ptr> trx_traces;
};
using block_trace_ptr = std::shared_ptr<block_trace>;

}}  // namespace vros::chain

FC_REFLECT(vros::chain::action_trace, (receipt)(act)(elapsed)(console)(trx_id))
FC_REFLECT(vros::chain::transaction_trace, (id)(receipt)(elapsed)(is_suspend)(action_traces)(charge)(except))
FC_REFLECT(vros::chain::block_trace, (elapsed)(trx_traces))
