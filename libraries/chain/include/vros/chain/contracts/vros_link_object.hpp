/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */
#pragma once
#include <fc/io/raw.hpp>

#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <vros/chain/multi_index_includes.hpp>

namespace vros { namespace chain { namespace contracts {

using boost::multi_index_container;
using namespace boost::multi_index;

class vros_link_object : public chainbase::object<vros_link_object_type, vros_link_object> {
    OBJECT_CTOR(vros_link_object)

    id_type             id;
    link_id_type        link_id;
    transaction_id_type trx_id;
};

struct by_link_id;
struct by_link_trx_id;
using vros_link_multi_index = chainbase::shared_multi_index_container<
    vros_link_object,
    indexed_by<
        ordered_unique<tag<by_id>, BOOST_MULTI_INDEX_MEMBER(vros_link_object, vros_link_object::id_type, id)>,
        ordered_unique<tag<by_link_id>, BOOST_MULTI_INDEX_MEMBER(vros_link_object, uint128_t, link_id)>,
        ordered_unique<tag<by_link_trx_id>, BOOST_MULTI_INDEX_MEMBER(vros_link_object, transaction_id_type, trx_id)>>>;

typedef chainbase::generic_index<vros_link_multi_index> vros_link_index;

}}}  // namespace vros::chain::contracts

CHAINBASE_SET_INDEX_TYPE(vros::chain::contracts::vros_link_object, vros::chain::contracts::vros_link_multi_index)
