/**
 *  @file
 *  @copyright defined in evt/LICENSE.txt
*/
#pragma once
#include <deque>
#include <boost/noncopyable.hpp>
#include <evt/chain/asset.hpp>
#include <evt/chain/contracts/types.hpp>
#include <functional>
#include <rocksdb/options.h>

namespace rocksdb {
class DB;
}  // namespace rocksdb

// Use only lower 48-bit address for some pointers.
/*
    This optimization uses the fact that current X86-64 architecture only implement lower 48-bit virtual address.
    The higher 16-bit can be used for storing other data.
*/
#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#define SETPOINTER(type, p, x) (p = reinterpret_cast<type *>((reinterpret_cast<uintptr_t>(p) & static_cast<uintptr_t>(0xFFFF000000000000UL)) | reinterpret_cast<uintptr_t>(reinterpret_cast<const void*>(x))))
#define GETPOINTER(type, p) (reinterpret_cast<type *>(reinterpret_cast<uintptr_t>(p) & static_cast<uintptr_t>(0x0000FFFFFFFFFFFFUL)))
#else
#error EVT can only be compiled in X86-64 architecture
#endif

namespace evt { namespace chain {

using namespace evt::chain::contracts;
using read_fungible_func = std::function<bool(const asset&)>;
using read_prodvote_func = std::function<bool(const public_key_type& pkey, int64_t value)>;

class token_database : boost::noncopyable {
public:
    struct flag {
    public:
        flag(uint16_t type) : type(type) {}

    public:
        char     payload[6];
        uint16_t type;
    };

    // realtime action
    // stored in memory
    struct rt_action {
    public:
        rt_action(uint16_t type, void* data)
            : f(type) {
            SETPOINTER(void, this->data, data);
        }

        union {
            flag  f;
            void* data;
        };
    };

    struct rt_group {
        const void*            rb_snapshot;
        std::vector<rt_action> actions;
    };

    // persistent action
    // stored in disk
    struct pd_action {
        uint16_t    op;
        uint16_t    type;
        std::string key;
        std::string value;
    };

    struct pd_group {
        int64_t                seq; // used for persistent
        std::vector<pd_action> actions;
    };

    struct sp_node {
    public:
        sp_node(uint16_t type) : f(type) {}

    public:
        union {
            flag  f;
            void* group;
        };
    };

    struct savepoint {
    public:
        savepoint(int64_t seq, uint16_t type)
            : seq(seq), node(type) {}

    public:
        int64_t  seq;
        sp_node  node;
    };

public:
    class session {
    public:
        session(token_database& token_db, int seq)
            : _token_db(token_db)
            , _seq(seq)
            , _accept(0) {}
        session(const session& s) = delete;
        session(session&& s)
            : _token_db(s._token_db)
            , _seq(s._seq)
            , _accept(s._accept) {
            if(!_accept) {
                s._accept = 1;
            }
        }

        ~session() {
            if(!_accept) {
                _token_db.rollback_to_latest_savepoint();
            }
        }

    public:
        void accept() { _accept = 1; }
        void squash() { _accept = 1; _token_db.squash(); }
        void undo()   { _accept = 1; _token_db.rollback_to_latest_savepoint(); }

        int seq() const { return _seq; }

    private:
        token_database& _token_db;
        int             _seq;
        int             _accept;
    };

public:
    token_database()
        : db_(nullptr)
        , read_opts_()
        , write_opts_()
        , tokens_handle_(nullptr)
        , assets_handle_(nullptr)
        , savepoints_() {}
    token_database(const fc::path& dbpath);
    ~token_database();

public:
    int initialize(const fc::path& dbpath);

public:
    int add_domain(const domain_def&);
    int exists_domain(const domain_name&) const;
    int issue_tokens(const issuetoken&);
    int exists_token(const domain_name&, const token_name&) const;
    int add_group(const group_def&);
    int exists_group(const group_name&) const;
    int add_suspend(const suspend_def&);
    int exists_suspend(const proposal_name&) const;
    int add_fungible(const fungible_def&);
    int exists_fungible(const symbol) const;
    int exists_fungible(const symbol_id_type) const;

    int update_asset(const address& addr, const asset&);
    int exists_any_asset(const address& addr) const;
    int exists_asset(const address& addr, const symbol) const;

    int update_prodvote(const conf_key& key, const public_key_type& pkey, int64_t value);

    int read_domain(const domain_name&, domain_def&) const;
    int read_token(const domain_name&, const token_name&, token_def&) const;
    int read_group(const group_name&, group_def&) const;
    int read_suspend(const proposal_name&, suspend_def&) const;

    int read_fungible(const symbol, fungible_def&) const;
    int read_fungible(const symbol_id_type, fungible_def&) const;
    int read_asset(const address& addr, const symbol, asset&) const;
    // this function returns asset(0, symbol) when there's no asset key in address
    // instead of throwing an exception
    int read_asset_no_throw(const address& addr, const symbol, asset&) const;
    int read_all_assets(const address& addr, const read_fungible_func&) const;

    int read_prodvotes_no_throw(const conf_key& key, const read_prodvote_func&) const;

    int update_domain(const domain_def&);
    int update_group(const group_def&);
    int update_token(const token_def&);
    int update_suspend(const suspend_def&);
    int update_fungible(const fungible_def&);

public:
    int add_savepoint(int64_t seq);
    int rollback_to_latest_savepoint();
    int pop_savepoints(int64_t until);
    int pop_back_savepoint();
    int squash();

    session new_savepoint_session(int64_t seq);
    session new_savepoint_session();

    size_t get_savepoints_size() const { return savepoints_.size(); }

private:
    int rollback_rt_group(rt_group*);
    int rollback_pd_group(pd_group*);

private:
    int should_record() { return !savepoints_.empty(); }
    int record(int type, void* data);
    int free_savepoint(savepoint&);

private:
    int persist_savepoints();
    int load_savepoints();

private:
    std::string                  db_path_;

    rocksdb::DB*                 db_;
    rocksdb::ReadOptions         read_opts_;
    rocksdb::WriteOptions        write_opts_;

    rocksdb::ColumnFamilyHandle* tokens_handle_;
    rocksdb::ColumnFamilyHandle* assets_handle_;  

    std::deque<savepoint>        savepoints_;
};

}}  // namespace evt::chain
