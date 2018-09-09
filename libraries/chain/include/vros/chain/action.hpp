/**
 *  @file
 *  @copyright defined in vros/LICENSE.txt
 */
#pragma once
#include <type_traits>
#include <boost/any.hpp>
#include <vros/chain/types.hpp>
#include <vros/chain/exceptions.hpp>

namespace vros { namespace chain {

struct action {
public:
    action_name name;
    domain_name domain;
    domain_key  key;
    bytes       data;

    action() {}

    template <typename T>
    action(const domain_name& domain, const domain_key& key, const T& value)
        : domain(domain)
        , key(key) {
        name   = T::get_name();
        data   = fc::raw::pack(value);
        cache_ = value;
    }

    action(const action_name name, const domain_name& domain, const domain_key& key, const bytes& data)
        : name(name)
        , domain(domain)
        , key(key)
        , data(data) {}

    // if T is a reference, will return the reference to the internal cache value
    // Otherwise if T is a value type, will return new copy. 
    template <typename T>
    T
    data_as() const {
        if(cache_.empty()) {
            using raw_type = std::remove_const_t<std::remove_reference_t<T>>;
            vros_ASSERT(name == raw_type::get_name(), action_type_exception, "action name is not consistent with action struct");
            cache_ = fc::raw::unpack<raw_type>(data);
        }
        // no need to check name here, `any_cast` will throws exception if types don't match
        return boost::any_cast<T>(cache_);
    }

private:
    mutable boost::any cache_;
};

}}  // namespace vros::chain

FC_REFLECT(vros::chain::action, (name)(domain)(key)(data))