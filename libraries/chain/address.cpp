/**
 *  @file
 *  @copyright defined in evt/LICENSE.txt
 */
#include <evt/chain/address.hpp>
#include <evt/chain/exceptions.hpp>
#include <fc/crypto/base58.hpp>
#include <fc/crypto/ripemd160.hpp>

namespace evt { namespace chain {

const std::string reserved_key = "EVT00000000000000000000000000000000000000000000000000";

namespace __internal {

struct gen_wrapper {
public:
    uint32_t  checksum;
    uint32_t  nonce;
    uint64_t  prefix;
    uint128_t key;

public:
    uint32_t
    calculate_checksum() {
         auto encoder = fc::ripemd160::encoder();
         encoder.write((const char*)&prefix, sizeof(prefix));
         encoder.write((const char*)&key, sizeof(key));
         encoder.write((const char*)&nonce, sizeof(nonce));

         checksum = encoder.result()._hash[0];
         return checksum;
    }
} __attribute__((packed));

}  // namespace __internal

size_t
address::get_bytes_size() const {
    using namespace __internal;
    
    constexpr size_t pkey_size = sizeof(public_key_type::storage_type::type_at<0>);
    static_assert(sizeof(gen_wrapper) <= pkey_size);
    return pkey_size;
}

void
address::to_bytes(char* buf, size_t sz) const {
    using namespace __internal;

    FC_ASSERT(sz == get_bytes_size());
    switch(this->type()) {
    case reserved_t: {
        memset(buf, 0, sz);
        break;
    }
    case public_key_t: {
        memcpy(buf, &this->get_public_key(), sizeof(public_key_type::storage_type::type_at<0>));
        break;
    }
    case generated_t: {
        memset(buf, 0, sz);

        auto gen = gen_wrapper();
        gen.prefix = this->get_prefix().value;
        gen.key = this->get_key().value;
        gen.nonce = this->get_nonce();
        gen.checksum = gen.calculate_checksum();

        memcpy(buf, &gen, sizeof(gen));
        break;
    }
    }  // switch
}

std::string
address::to_string() const {
    using namespace __internal;

    switch(type()) {
    case reserved_t: {
        return reserved_key;
    }
    case public_key_t: {
        return (std::string)this->get_public_key();
    }
    case generated_t: {
        auto str = std::string();
        str.reserve(53);

        str.append("EVT0");

        auto gen = gen_wrapper();
        gen.prefix = this->get_prefix().value;
        gen.key = this->get_key().value;
        gen.nonce = this->get_nonce();
        gen.checksum = gen.calculate_checksum();

        auto hash = fc::to_base58((char*)&gen, sizeof(gen));
        EVT_ASSERT(hash.size() <= 53 - 4, address_type_exception, "Invalid generated values for address");

        str.append(53 - 4 - hash.size(), '0');
        str.append(std::move(hash));

        return str;
    }
    default: {
        EVT_ASSERT(false, address_type_exception, "Not valid address type: ${type}", ("type",type()));
    }
    }  // switch
}

address
address::from_string(const std::string& str) {
    using namespace __internal;
    EVT_ASSERT(str.size() == 53, address_type_exception, "Address is not valid");

    address addr;
    // fast path
    if(str[3] != '0') {
        addr.set_public_key((public_key_type)str);
        return addr;
    }

    if(str == reserved_key) {
        addr.set_reserved();
        return addr;
    }

    auto gen = gen_wrapper();
    auto hash = str.substr(str.find_first_not_of('0', 4));
    fc::from_base58(hash, (char*)&gen, sizeof(gen));

    EVT_ASSERT(gen.checksum == gen.calculate_checksum(), address_type_exception, "Checksum doesn't match");

    addr.set_generated(gen.prefix, gen.key, gen.nonce);
    return addr;
}

}}  // namespace evt::chain

namespace fc {

void
to_variant(const evt::chain::address& addr, fc::variant& v) {
    v = addr.to_string();
}

void
from_variant(const fc::variant& v, evt::chain::address& addr) {
    addr = evt::chain::address::from_string(v.get_string());
}

}  // namespace fc