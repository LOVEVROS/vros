/**
 *  @file
 *  @copyright defined in evt/LICENSE.txt
 */
#pragma once
#include <fc/container/flat_fwd.hpp>
#include <fc/static_variant.hpp>
#include <fc/optional.hpp>
#include <fc/variant.hpp>
#include <fc/reflect/reflect.hpp>
#include <fc/crypto/sha256.hpp>
#include <evt/chain/types.hpp>

namespace evt { namespace chain { namespace contracts {

class evt_link {
public:
    struct segment {
        segment() = default;
        
        segment(uint8_t key, uint32_t intv)
            : key(key), intv(intv) {}

        segment(uint8_t key, const std::string& strv)
            : key(key), strv(strv) {}

        uint8_t                   key;
        fc::optional<uint32_t>    intv;
        fc::optional<std::string> strv;
    };

public:
    enum type_id {
        timestamp   = 42,
        max_pay     = 43,
        symbol_id   = 44,
        domain      = 91,
        token       = 92,
        max_pay_str = 94,
        address     = 95,
        link_id     = 156
    };

    enum flag {
        version1  = 1,
        everiPass = 2,
        everiPay  = 4,
        destroy   = 8
    };

public:
    static evt_link parse_from_evtli(const std::string& str);

public:
    uint16_t get_header() const { return header_; }

    const segment& get_segment(uint8_t key) const;
    bool has_segment(uint8_t key) const;

    const fc::flat_map<uint8_t, segment>& get_segments() const { return segments_; }
    const fc::flat_set<signature_type>& get_signatures() const {return signatures_; }

public:
    void set_header(uint16_t header) { header_ = header; }
    void add_segment(const segment&);
    void add_signature(const signature_type&);
    void clear_signatures() { signatures_.clear(); }
    void sign(const private_key_type&);

public:
    fc::sha256 digest() const;
    fc::flat_set<public_key_type> restore_keys() const;

private:
    uint16_t                       header_;
    fc::flat_map<uint8_t, segment> segments_;
    fc::flat_set<signature_type>   signatures_;

private:
    friend struct fc::reflector<evt_link>;
};

}}}  // namespac evt::chain::contracts

namespace fc {

class variant;
void to_variant(const evt::chain::contracts::evt_link& link, fc::variant& v);
void from_variant(const fc::variant& v, evt::chain::contracts::evt_link& link);

}  // namespace fc

FC_REFLECT(evt::chain::contracts::evt_link::segment, (key)(intv)(strv));
FC_REFLECT(evt::chain::contracts::evt_link, (header_)(segments_)(signatures_));
