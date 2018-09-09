/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */
#include <vros/chain/contracts/vros_contract.hpp>
#include <vros/chain/contracts/types.hpp>
#include <vros/chain/contracts/abi_types.hpp>
#include <vros/chain/version.hpp>

namespace vros { namespace chain { namespace contracts {

static auto vros_abi_version       = 3;
static auto vros_abi_minor_version = 1;
static auto vros_abi_patch_version = 1;

version
vros_contract_abi_version() {
    return version(vros_abi_version, vros_abi_minor_version, vros_abi_patch_version);
}

abi_def
vros_contract_abi() {
    abi_def vros_abi;
    vros_abi.types.push_back( type_def{"address_list","address[]"} );
    vros_abi.types.push_back( type_def{"user_id","public_key"} );
    vros_abi.types.push_back( type_def{"user_list","public_key[]"} );
    vros_abi.types.push_back( type_def{"group_key","public_key"} );
    vros_abi.types.push_back( type_def{"weight_type","uint16"} );
    vros_abi.types.push_back( type_def{"fields","field_def[]"} );
    vros_abi.types.push_back( type_def{"type_name","string"} );
    vros_abi.types.push_back( type_def{"field_name","string"} );
    vros_abi.types.push_back( type_def{"permission_name","name"} );
    vros_abi.types.push_back( type_def{"action_name","name"} );
    vros_abi.types.push_back( type_def{"domain_name","name128"} );
    vros_abi.types.push_back( type_def{"domain_key","name128"} );
    vros_abi.types.push_back( type_def{"group_name","name128"} );
    vros_abi.types.push_back( type_def{"token_name","name128"} );
    vros_abi.types.push_back( type_def{"account_name","name128"} );
    vros_abi.types.push_back( type_def{"proposal_name","name128"} );
    vros_abi.types.push_back( type_def{"fungible_name","name128"} );
    vros_abi.types.push_back( type_def{"symbol_name","name128"} );
    vros_abi.types.push_back( type_def{"symbol_id_type","uint32"} );
    vros_abi.types.push_back( type_def{"balance_type","asset"} );
    vros_abi.types.push_back( type_def{"group_def","group"} );
    vros_abi.types.push_back( type_def{"meta_key","name128"} );
    vros_abi.types.push_back( type_def{"meta_value","string"} );
    vros_abi.types.push_back( type_def{"meta_list","meta[]"} );
    vros_abi.types.push_back( type_def{"suspend_status","uint8"} );
    vros_abi.types.push_back( type_def{"conf_key","name128"} );

    vros_abi.actions.push_back( action_def{name("newdomain"), "newdomain"} );
    vros_abi.actions.push_back( action_def{name("issuetoken"), "issuetoken"} );
    vros_abi.actions.push_back( action_def{name("transfer"), "transfer"} );
    vros_abi.actions.push_back( action_def{name("destroytoken"), "destroytoken"} );
    vros_abi.actions.push_back( action_def{name("newgroup"), "newgroup"} );
    vros_abi.actions.push_back( action_def{name("updategroup"), "updategroup"} );
    vros_abi.actions.push_back( action_def{name("updatedomain"), "updatedomain"} );
    vros_abi.actions.push_back( action_def{name("newfungible"), "newfungible"} );
    vros_abi.actions.push_back( action_def{name("updfungible"), "updfungible"} );
    vros_abi.actions.push_back( action_def{name("issuefungible"), "issuefungible"} );
    vros_abi.actions.push_back( action_def{name("transferft"), "transferft"} );
    vros_abi.actions.push_back( action_def{name("vros2pvros"), "vros2pvros"} );
    vros_abi.actions.push_back( action_def{name("addmeta"), "addmeta"} );
    vros_abi.actions.push_back( action_def{name("newsuspend"), "newsuspend"} );
    vros_abi.actions.push_back( action_def{name("cancelsuspend"), "cancelsuspend"} );
    vros_abi.actions.push_back( action_def{name("aprvsuspend"), "aprvsuspend"} );
    vros_abi.actions.push_back( action_def{name("execsuspend"), "execsuspend"} );
    vros_abi.actions.push_back( action_def{name("paycharge"), "paycharge"} );
    vros_abi.actions.push_back( action_def{name("everipass"), "everipass"} );
    vros_abi.actions.push_back( action_def{name("everipay"), "everipay"} );
    vros_abi.actions.push_back( action_def{name("prodvote"), "prodvote"} );

    // structures def
    vros_abi.structs.emplace_back( struct_def {
        "meta", "", {
            {"key", "meta_key"},
            {"value", "meta_value"},
            {"creator", "user_id"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "token_def", "", {
            {"domain", "domain_name"},
            {"name", "token_name"},
            {"owner", "address_list"},
            {"metas", "meta_list"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "key_weight", "", {
            {"key", "public_key"},
            {"weight", "weight_type"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "authorizer_weight", "", {
            {"ref", "authorizer_ref"},
            {"weight", "weight_type"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "permission_def", "", {
            {"name", "permission_name"},
            {"threshold", "uint32"},
            {"authorizers", "authorizer_weight[]"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "domain_def", "", {
            {"name", "domain_name"},
            {"creator", "user_id"},
            {"create_time", "time_point_sec"},
            {"issue", "permission_def"},
            {"transfer", "permission_def"},
            {"manage", "permission_def"},
            {"metas", "meta_list"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "fungibal_def", "", {
            {"sym", "symbol"},
            {"creator", "user_id"},
            {"create_time", "time_point_sec"},
            {"issue", "permission_def"},
            {"manage", "permission_def"},
            {"total_supply", "asset"},
            {"metas", "meta_list"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "suspend_def", "", {
            {"name", "proposal_name"},
            {"proposer", "user_id"},
            {"status", "suspend_status"},
            {"trx", "transaction"},
            {"signed_keys","public_key[]"},
            {"signatures","signature[]"}
        }
    });

    // actions def
    vros_abi.structs.emplace_back( struct_def {
        "newdomain", "", {
            {"name", "domain_name"},
            {"creator", "user_id"},
            {"issue", "permission_def"},
            {"transfer", "permission_def"},
            {"manage", "permission_def"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "issuetoken", "", {
            {"domain", "domain_name"},
            {"names", "token_name[]"},
            {"owner", "address_list"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "transfer", "", {
            {"domain", "domain_name"},
            {"name", "token_name"},
            {"to", "address_list"},
            {"memo", "string"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "destroytoken", "", {
            {"domain", "domain_name"},
            {"name", "token_name"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "newgroup", "", {
            {"name", "group_name"},
            {"group", "group_def"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "updategroup", "", {
            {"name", "group_name"},
            {"group", "group_def"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "updatedomain", "", {
            {"name", "domain_name"},
            {"issue", "permission_def?"},
            {"transfer", "permission_def?"},
            {"manage", "permission_def?"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "newfungible", "", {
            {"name", "fungible_name"},
            {"sym_name", "symbol_name"},
            {"sym", "symbol"},
            {"creator", "user_id"},
            {"issue", "permission_def"},
            {"manage", "permission_def"},
            {"total_supply", "asset"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "updfungible", "", {
            {"sym_id", "symbol_id_type"},
            {"issue", "permission_def?"},
            {"manage", "permission_def?"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "issuefungible", "", {
            {"address", "address"},
            {"number", "asset"},
            {"memo", "string"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "transferft", "", {
            {"from", "address"},
            {"to", "address"},
            {"number", "asset"},
            {"memo", "string"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "vros2pvros", "", {
            {"from", "address"},
            {"to", "address"},
            {"number", "asset"},
            {"memo", "string"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "addmeta", "", {
            {"key", "meta_key"},
            {"value", "meta_value"},
            {"creator", "authorizer_ref"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "newsuspend", "", {
            {"name", "proposal_name"},
            {"proposer", "user_id"},
            {"trx", "transaction"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "cancelsuspend", "", {
            {"name", "proposal_name"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "aprvsuspend", "", {
            {"name", "proposal_name"},
            {"signatures", "signature[]"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "execsuspend", "", {
           {"name", "proposal_name"},
           {"executor", "user_id"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "paycharge", "", {
           {"payer", "address"},
           {"charge", "uint32"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "everipass", "", {
           {"link", "vros_link"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "everipay", "", {
           {"link", "vros_link"},
           {"payee", "address"},
           {"number", "asset"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "prodvote", "", {
           {"producer", "account_name"},
           {"key", "conf_key"},
           {"value", "int64"}
        }
    });

    // abi_def fields
    vros_abi.structs.emplace_back( struct_def {
        "field_def", "", {
            {"name", "field_name"},
            {"type", "type_name"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "struct_def", "", {
            {"name", "type_name"},
            {"base", "type_name"},
            {"fields", "fields"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "type_def", "", {
            {"new_type_name", "type_name"},
            {"type", "type_name"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "action_def", "", {
           {"name", "action_name"},
           {"type", "type_name"}
        }
    });

    // blocks & transactions def
    vros_abi.structs.emplace_back( struct_def {
        "action", "", {
            {"name", "action_name"},
            {"domain", "domain_name"},
            {"key", "domain_key"},
            {"data", "bytes"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "transaction_header", "", {
            {"expiration", "time_point_sec"},
            {"ref_block_num", "uint16"},
            {"ref_block_prefix", "uint32"},
            {"max_charge", "uint32"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "transaction", "transaction_header", {
            {"actions", "action[]"},
            {"payer", "address"},
            {"transaction_extensions", "extensions"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "signed_transaction", "transaction", {
            {"signatures", "signature[]"}
        }
    });

    vros_abi.structs.emplace_back( struct_def {
        "block_header", "", {
            {"previous", "checksum256"},
            {"timestamp", "uint32"},
            {"transaction_mroot", "checksum256"},
            {"action_mroot", "checksum256"},
            {"block_mroot", "checksum256"},
            {"producer", "account_name"},
            {"schedule_version", "uint32"},
            {"new_producers", "producer_schedule?"}
        }
    });

    return vros_abi;
}

}}}  // namespace vros::chain::contracts