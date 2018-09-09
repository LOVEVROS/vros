/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */
#include <vros/chain/contracts/vros_org.hpp>
#include <vros/chain/token_database.hpp>
#include <vros/chain/genesis_state.hpp>

namespace vros { namespace chain { namespace contracts {

void
initialize_vros_org(token_database& token_db, const genesis_state& genesis) {
    // Add reserved everiToken foundation group
    if(!token_db.exists_group(".everiToken")) {
        token_db.add_group(genesis.vros_org);
    }

    // Add reserved vros fungible tokens
    if(!token_db.exists_fungible(vros_sym())) {
        token_db.add_fungible(genesis.vros);

        auto addr = address(N(fungible), (name128)std::to_string(vros_sym().id()), 0);
        token_db.update_asset(addr, genesis.vros.total_supply);
    }

    // Add reserved Pined vros fungible tokens
    if(!token_db.exists_fungible(pvros_sym())) {
        token_db.add_fungible(genesis.pvros);
    }
}

}}}  // namespace vros::chain::contracts
