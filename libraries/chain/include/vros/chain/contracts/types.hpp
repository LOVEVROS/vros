/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */
#pragma once

#include <vros/chain/address.hpp>
#include <vros/chain/asset.hpp>
#include <vros/chain/chain_config.hpp>
#include <vros/chain/config.hpp>
#include <vros/chain/types.hpp>
#include <vros/chain/transaction.hpp>

#include <vros/chain/contracts/authorizer_ref.hpp>
#include <vros/chain/contracts/group.hpp>
#include <vros/chain/contracts/metadata.hpp>
#include <vros/chain/contracts/vros_link.hpp>

namespace vros { namespace chain { namespace contracts {

#define vros_ACTION(typename)       \
    typename() = default;          \
                                   \
    static constexpr action_name   \
    get_name() {                   \
        return N(typename);        \
    }

using domain_name     = vros::chain::domain_name;
using domian_key      = vros::chain::domain_key;
using token_name      = vros::chain::token_name;
using permission_name = vros::chain::permission_name;
using account_name    = vros::chain::account_name;
using fungible_name   = vros::chain::fungible_name;
using symbol_name     = vros::chain::symbol_name;
using symbol_id_type  = vros::chain::symbol_id_type;
using user_id         = vros::chain::public_key_type;
using user_list       = std::vector<user_id>;
using group_name      = vros::chain::group_name;
using group_key       = vros::chain::address;
using group_def       = group;
using balance_type    = vros::chain::asset;
using address_type    = vros::chain::address;
using address_list    = std::vector<address_type>;
using conf_key        = vros::chain::conf_key;

struct token_def {
    token_def() = default;
    token_def(const domain_name& domain, const token_name& name, const address_list& owner)
        : domain(domain)
        , name(name)
        , owner(owner) {}

    domain_name  domain;
    token_name   name;
    address_list owner;

    meta_list metas;
};

struct key_weight {
    public_key_type key;
    weight_type     weight;
};

struct authorizer_weight {
    authorizer_weight() = default;
    authorizer_weight(const authorizer_ref& ref, weight_type weight)
        : ref(ref), weight(weight) {}

    authorizer_ref ref;
    weight_type    weight;
};

struct permission_def {
    permission_def() = default;

    permission_name           name;
    uint32_t                  threshold;
    vector<authorizer_weight> authorizers;
};

struct domain_def {
    domain_def() = default;

    domain_name    name;
    user_id        creator;
    time_point_sec create_time;

    permission_def issue;
    permission_def transfer;
    permission_def manage;

    meta_list metas;
};

struct fungible_def {
    fungible_def() = default;

    fungible_name  name;
    symbol_name    sym_name;
    symbol         sym;
    
    user_id        creator;
    time_point_sec create_time;

    permission_def issue;
    permission_def manage;

    asset total_supply;
    
    meta_list metas;
};

enum suspend_status {
    proposed = 0, executed, failed, cancelled
};

struct suspend_def {
    suspend_def() = default;

    proposal_name                          name;
    public_key_type                        proposer;
    fc::enum_type<uint8_t, suspend_status> status;
    transaction                            trx;
    flat_set<public_key_type>              signed_keys;
};

struct newdomain {
    domain_name name;
    user_id     creator;

    permission_def issue;
    permission_def transfer;
    permission_def manage;

    vros_ACTION(newdomain);
};

struct issuetoken {
    domain_name             domain;
    std::vector<token_name> names;
    address_list            owner;

    vros_ACTION(issuetoken);
};

struct transfer {
    domain_name  domain;
    token_name   name;
    address_list to;
    string       memo;

    vros_ACTION(transfer);
};

struct destroytoken {
    domain_name domain;
    token_name  name;

    vros_ACTION(destroytoken);
};

struct newgroup {
    group_name name;
    group_def  group;

    vros_ACTION(newgroup);
};

struct updategroup {
    group_name name;
    group_def  group;

    vros_ACTION(updategroup);
};

struct updatedomain {
    domain_name name;

    fc::optional<permission_def> issue;
    fc::optional<permission_def> transfer;
    fc::optional<permission_def> manage;

    vros_ACTION(updatedomain);
};

struct newfungible {
    fungible_name name;
    symbol_name   sym_name;
    symbol        sym;
    user_id       creator;

    permission_def issue;
    permission_def manage;

    asset total_supply;

    vros_ACTION(newfungible);
};

struct updfungible {
    symbol_id_type sym_id;

    fc::optional<permission_def> issue;
    fc::optional<permission_def> manage;

    vros_ACTION(updfungible);
};

struct issuefungible {
    address_type address;
    asset        number;
    string       memo;

    vros_ACTION(issuefungible);
};

struct transferft {
    address_type from;
    address_type to;
    asset        number;
    string       memo;

    vros_ACTION(transferft);
};

struct vros2pvros {
    address_type from;
    address_type to;
    asset        number;
    string       memo;

    vros_ACTION(vros2pvros);
};

struct addmeta {
    meta_key       key;
    meta_value     value;
    authorizer_ref creator;

    vros_ACTION(addmeta);
};

struct newsuspend {
    proposal_name name;
    user_id       proposer;
    transaction   trx;

    vros_ACTION(newsuspend);
};

struct cancelsuspend {
    proposal_name name;

    vros_ACTION(cancelsuspend);
};

struct aprvsuspend {
    proposal_name               name;
    std::vector<signature_type> signatures;

    vros_ACTION(aprvsuspend);
};

struct execsuspend {
    proposal_name name;
    user_id       executor;

    vros_ACTION(execsuspend);
};

struct paycharge {
    address  payer;
    uint32_t charge;

    vros_ACTION(paycharge);
};

struct everipass {
    vros_link link;

    vros_ACTION(everipass);
};

struct everipay {
    vros_link link;
    address  payee;
    asset    number;

    vros_ACTION(everipay);
};

struct prodvote {
    account_name producer;
    conf_key     key;
    int64_t      value;

    vros_ACTION(prodvote);
};

}}}  // namespace vros::chain::contracts

FC_REFLECT(vros::chain::contracts::token_def, (domain)(name)(owner)(metas));
FC_REFLECT(vros::chain::contracts::key_weight, (key)(weight));
FC_REFLECT(vros::chain::contracts::authorizer_weight, (ref)(weight));
FC_REFLECT(vros::chain::contracts::permission_def, (name)(threshold)(authorizers));
FC_REFLECT(vros::chain::contracts::domain_def, (name)(creator)(create_time)(issue)(transfer)(manage)(metas));
FC_REFLECT(vros::chain::contracts::fungible_def, (name)(sym_name)(sym)(creator)(create_time)(issue)(manage)(total_supply)(metas));
FC_REFLECT_ENUM(vros::chain::contracts::suspend_status, (proposed)(executed)(failed)(cancelled));
FC_REFLECT(vros::chain::contracts::suspend_def, (name)(proposer)(status)(trx)(signed_keys));

FC_REFLECT(vros::chain::contracts::newdomain, (name)(creator)(issue)(transfer)(manage));
FC_REFLECT(vros::chain::contracts::issuetoken, (domain)(names)(owner));
FC_REFLECT(vros::chain::contracts::transfer, (domain)(name)(to)(memo));
FC_REFLECT(vros::chain::contracts::destroytoken, (domain)(name));
FC_REFLECT(vros::chain::contracts::newgroup, (name)(group));
FC_REFLECT(vros::chain::contracts::updategroup, (name)(group));
FC_REFLECT(vros::chain::contracts::updatedomain, (name)(issue)(transfer)(manage));
FC_REFLECT(vros::chain::contracts::newfungible, (name)(sym_name)(sym)(creator)(issue)(manage)(total_supply));
FC_REFLECT(vros::chain::contracts::updfungible, (sym_id)(issue)(manage));
FC_REFLECT(vros::chain::contracts::issuefungible, (address)(number)(memo));
FC_REFLECT(vros::chain::contracts::transferft, (from)(to)(number)(memo));
FC_REFLECT(vros::chain::contracts::vros2pvros, (from)(to)(number)(memo));
FC_REFLECT(vros::chain::contracts::addmeta, (key)(value)(creator));
FC_REFLECT(vros::chain::contracts::newsuspend, (name)(proposer)(trx));
FC_REFLECT(vros::chain::contracts::cancelsuspend, (name));
FC_REFLECT(vros::chain::contracts::aprvsuspend, (name)(signatures));
FC_REFLECT(vros::chain::contracts::execsuspend, (name)(executor));
FC_REFLECT(vros::chain::contracts::paycharge, (payer)(charge));
FC_REFLECT(vros::chain::contracts::everipass, (link));
FC_REFLECT(vros::chain::contracts::everipay, (link)(payee)(number));
FC_REFLECT(vros::chain::contracts::prodvote, (producer)(key)(value));
