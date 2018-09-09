/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */

#include <vros/chain/chain_config.hpp>
#include <vros/chain/exceptions.hpp>

namespace vros { namespace chain {

void 
chain_config::validate()const {
    vros_ASSERT(1 <= max_authority_depth, action_exception,
              "max authority depth should be at least 1");
}

}}  // namespace vros::chain