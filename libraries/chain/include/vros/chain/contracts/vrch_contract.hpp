/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */
#pragma once

namespace vros { namespace chain {
class apply_context;
class version;
}}  // namespace vros::chain

namespace vros { namespace chain { namespace contracts {

struct abi_def;

template<typename T>
struct apply_action {
    static void invoke(apply_context&);
};

abi_def vros_contract_abi();
version vros_contract_abi_version();

}}}  // namespace vros::chain::contracts
