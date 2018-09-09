/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */
#pragma once
#include <vros/chain/block_header.hpp>
#include <vros/chain/transaction.hpp>

namespace vros { namespace chain {

/**
 * When a transaction is referenced by a block it could imply one of several
 * outcomes which describe the state-transition undertaken by the block
 * producer.
 */

struct transaction_receipt_header {
    enum status_enum {
        executed  = 0,  ///< succeed
        soft_fail = 1,  ///< objectively failed (not executed)
        hard_fail = 2,  ///< objectively failed and error handler objectively
                        ///< failed thus no state change
        expired = 3     ///< transaction expired and storage space refuned to user
    };
    enum type_enum {
        input = 0,    /// Normal transaction, pushed by user
        suspend = 1   /// Suspend transaction, generated by vros contract
    };

    transaction_receipt_header() : status(hard_fail), type(input) {}
    transaction_receipt_header(status_enum s, type_enum t = input) : status(s), type(t) {}

    friend inline bool
    operator ==(const transaction_receipt_header& lhs, const transaction_receipt_header& rhs) {
        return std::tie(lhs.status, lhs.type) == std::tie(rhs.status, rhs.type);
    }

    fc::enum_type<uint8_t, status_enum> status;
    fc::enum_type<uint8_t, type_enum>   type;
};

struct transaction_receipt : public transaction_receipt_header {
    transaction_receipt(): transaction_receipt_header() {}
    transaction_receipt(packed_transaction ptrx) : transaction_receipt_header(executed), trx(ptrx) {}

    packed_transaction trx;

    digest_type
    digest() const {
        digest_type::encoder enc;
        fc::raw::pack(enc, status);
        fc::raw::pack(enc, type);
        fc::raw::pack(enc, trx.packed_digest());
        return enc.result();
    }
};

/**
 */
struct signed_block : public signed_block_header {
    using signed_block_header::signed_block_header;
    signed_block() = default;
    signed_block(const signed_block_header& h) : signed_block_header(h) {}

    vector<transaction_receipt> transactions;  /// new or generated transactions
    extensions_type             block_extensions;
};
using signed_block_ptr = std::shared_ptr<signed_block>;

struct producer_confirmation {
    block_id_type  block_id;
    digest_type    block_digest;
    account_name   producer;
    signature_type sig;
};

}}  // namespace vros::chain

FC_REFLECT_ENUM(vros::chain::transaction_receipt::status_enum, (executed)(soft_fail)(hard_fail)(expired))
FC_REFLECT_ENUM(vros::chain::transaction_receipt::type_enum, (input)(suspend))

FC_REFLECT(vros::chain::transaction_receipt_header, (status)(type))
FC_REFLECT_DERIVED(vros::chain::transaction_receipt, (vros::chain::transaction_receipt_header), (trx))
FC_REFLECT_DERIVED(vros::chain::signed_block, (vros::chain::signed_block_header), (transactions)(block_extensions))
