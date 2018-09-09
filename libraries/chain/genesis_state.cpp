/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */
#include <vros/chain/genesis_state.hpp>
#include <vros/chain/contracts/types.hpp>
#include <fc/io/json.hpp>

namespace vros { namespace chain {

namespace __internal {

using namespace contracts;

group_def
get_vros_org() {

#ifndef MAINNET_BUILD
    const char* def = R"(
    {
        "name": ".everiToken",
        "key": "vros00000000000000000000000000000000000000000000000000",
        "root": {
            "threshold": 19,
            "nodes": [
                { "weight": 8, "key": "vros6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV" },
                { "weight": 7, "key": "vros6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV" },
                { "weight": 5, "key": "vros6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV" },
                { "weight": 4, "key": "vros6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV" },
                { "weight": 4, "key": "vros6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV" }
            ]
        }
    }
    )";
#else
    const char* def = R"(
    {
        "name": ".everiToken",
        "key": "vros00000000000000000000000000000000000000000000000000",
        "root": {
            "threshold": 19,
            "nodes": [
                { "weight": 8, "key": "vros6ZVMb3e69umQB4DQErvovx4fpy4ri2qMRmWnCjqCHRvzeWBYix" },
                { "weight": 7, "key": "vros8C5q7W6tieUb1z5e9NV9ohWorWKfHykZp46nVaqabNm5xPSpVe" },
                { "weight": 5, "key": "vros8PwjEmVji6xtNZdv8pNUuQyDavDyDcCQFDTZHDV4G6Vk9SMJUT" },
                { "weight": 4, "key": "vros6J3hLMqwVMpeCcQh74LJhVs9f23HHjr4AZBUTd9GtTMc7dgGeP" },
                { "weight": 4, "key": "vros8MSR6xwSoeDPAQDNZBTkDPvVjwEbuuiysMxdcMAz354WVaxCQu" }
            ]
        }
    }
    )";
#endif

    auto var = fc::json::from_string(def);
    return var.as<group_def>();
}

fungible_def
get_vros_sym(const genesis_state& genesis) {
    auto vros = fungible_def();
    vros.name = "vros";
    vros.sym_name = "vros";
    vros.sym = vros_sym();
    vros.creator = genesis.initial_key;
    vros.create_time = genesis.initial_timestamp;

    auto issue = permission_def();
    issue.name = N(issue);
    issue.threshold = 1;
    issue.authorizers.emplace_back(authorizer_weight(authorizer_ref(group_name(N128(.everiToken))), 1));

    auto manage = permission_def();
    manage.name = N(manage);
    manage.threshold = 0;

    vros.issue  = issue;
    vros.manage = manage;

    vros.total_supply = asset(100'000'000'000'000L, vros.sym);

    return vros;
}

fungible_def
get_pvros_sym(const genesis_state& genesis) {
    auto pvros = fungible_def();
    pvros.name = "Pinned.vros";
    pvros.sym_name = "Pvros";
    pvros.sym = pvros_sym();
    pvros.creator = genesis.initial_key;
    pvros.create_time = genesis.initial_timestamp;

    auto issue = permission_def();
    issue.name = N(issue);
    issue.threshold = 0;

    auto manage = permission_def();
    manage.name = N(manage);
    manage.threshold = 0;

    pvros.issue  = issue;
    pvros.manage = manage;

    pvros.total_supply = asset(0, pvros.sym);

    return pvros;
}

}  // namespace __internal

genesis_state::genesis_state() {
    using namespace __internal;

    initial_timestamp = fc::time_point::from_iso_string("2018-05-31T12:00:00");
    initial_key       = fc::variant(vros_root_key).as<public_key_type>();

    vros_org = get_vros_org();
    vros     = get_vros_sym(*this);
    pvros    = get_pvros_sym(*this);
}

chain::chain_id_type
genesis_state::compute_chain_id() const {
    digest_type::encoder enc;
    fc::raw::pack(enc, *this);
    return chain_id_type{enc.result()};
}

}}  // namespace vros::chain