/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */
#pragma once

#include <string>
#include <functional>
#include <fc/reflect/reflect.hpp>
#include <vros/chain/types.hpp>
#include <vros/chain/address.hpp>
#include <vros/chain/contracts/metadata.hpp>

namespace vros { namespace chain { namespace contracts {

class group {
public:
    struct node {
    public:
        weight_type weight;     // for root node, its weight is zero
        weight_type threshold;  // for leaf node, its threshold is zero
        uint16_t    index;      // for leaf node, it's the key index, otherwise is the node index
        uint16_t    size;       // not for leaf node, indicates how many child nodes it has

    public:
        bool is_root() const { return weight == 0; }
        bool is_leaf() const { return threshold == 0; }

    public:
        bool validate() const {
            if(is_root()) {
                return threshold > 0 && index > 0 && size > 0;
            }
            if(is_leaf()) {
                return index >= 0 && size == 0;
            }
            return index > 0 && size > 0;
        }
    };

    using visit_func = std::function<bool(const node&)>;

public:
    const group_name& name() const { return name_; }
    const address& key() const { return key_; }
    const node& root() const { FC_ASSERT(nodes_.size() > 0); return nodes_[0]; }
    const bool empty() const { return nodes_.empty(); }
    const auto metas() const { return metas_; }

public:
    void visit_root(const visit_func&) const;
    void visit_node(const node&, const visit_func&) const;

public:
    const public_key_type&
    get_leaf_key(const node& n) const {
        FC_ASSERT(n.is_leaf());
        return keys_[n.index];
    }

    const node&
    get_child_node(const node& n, int i) const {
        FC_ASSERT(!n.is_leaf());
        FC_ASSERT(i < n.size);
        return nodes_[n.index + i];
    }

public:
    group_name                      name_;
    address                         key_;
    std::vector<node>               nodes_;
    std::vector<public_key_type>    keys_;
    meta_list                       metas_;
};

}}}  // namespac vros::chain::contracts

namespace fc {

class variant;
void to_variant(const vros::chain::contracts::group& group, fc::variant& v);
void from_variant(const fc::variant& v, vros::chain::contracts::group& group);

}  // namespace fc

FC_REFLECT(vros::chain::contracts::group::node, (weight)(threshold)(index)(size))
FC_REFLECT(vros::chain::contracts::group, (name_)(key_)(nodes_)(keys_)(metas_))
