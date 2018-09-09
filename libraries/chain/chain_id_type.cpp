/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */

#include <vros/chain/chain_id_type.hpp>
#include <vros/chain/exceptions.hpp>

namespace vros { namespace chain {

void
chain_id_type::reflector_verify() const {
    vros_ASSERT(*reinterpret_cast<const fc::sha256*>(this) != fc::sha256(), chain_id_type_exception, "chain_id_type cannot be zero");
}

}}  // namespace vros::chain

namespace fc {

void
to_variant(const vros::chain::chain_id_type& cid, fc::variant& v) {
    to_variant(static_cast<const fc::sha256&>(cid), v);
}

void
from_variant(const fc::variant& v, vros::chain::chain_id_type& cid) {
    from_variant(v, static_cast<fc::sha256&>(cid));
}

}  // namespace fc