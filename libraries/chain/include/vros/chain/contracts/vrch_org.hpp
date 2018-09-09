/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */
#pragma once

namespace vros { namespace chain {
class token_database;
struct genesis_state;
}}  // namespace vros::chain

namespace vros { namespace chain { namespace contracts {

void initialize_vros_org(token_database& token_db, const genesis_state& genesis);

}}}  // namespace vros::chain::contracts
