/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */
#include <vros/chain/name128.hpp>
#include <boost/algorithm/string.hpp>
#include <vros/chain/exceptions.hpp>
#include <fc/variant.hpp>

namespace vros { namespace chain {

void
name128::set(const char* str) {
    const auto len = strnlen(str, 22);
    vros_ASSERT(len <= 21, name128_type_exception, "Name128 is longer than 21 characters (${name}) ",
               ("name", string(str)));
    value = string_to_name128(str);
    vros_ASSERT(to_string() == string(str), name128_type_exception,
               "Name128 not properly normalized (name: ${name}, normalized: ${normalized}) ",
               ("name", string(str))("normalized", to_string()));
}

name128::operator string() const {
    static const char* charmap = ".-0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    auto str = string(21, '.');
    
    auto tmp = value;
    tmp >>= 2;
    for(uint32_t i = 0; i <= 20; ++i) {
        auto c = charmap[tmp & 0x3f];
        str[i] = c;
        tmp >>= 6;
    }

    str.erase(str.find_last_not_of('.') + 1);
    return str;
}

}}  // namespace vros::chain


namespace fc {

void
to_variant(const vros::chain::name128& name, fc::variant& v) {
    v = std::string(name);
}
void
from_variant(const fc::variant& v, vros::chain::name128& name) {
    name = v.get_string();
}

}  // namespace fc
