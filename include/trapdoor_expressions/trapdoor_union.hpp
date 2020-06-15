#include "trapdoor.hpp"
#include "trapdoor_set.hpp"

/**
 * A trapdoor_union<X> is a trapdoor<powerset<X>>.
 */
template <typename X>
struct trapdoor_union: public trapdoor_set<X>
{
    using value_type = X;

    trapdoor_union() : value_hash(0), key_hash(0) {}
    trapdoor_union(trapdoor_union const & u) :
        value_hash(u.value_hash, u.key_hash) {}

    unsigned int value_hash;
    unsigned int key_hash;
};


template <typename X>
optional<trapdoor_union<X>> operator+(
    trapdoor_union<X> const & x,
    trapdoor_union<X> const & y)
{
    if (x.key_hash != y.key_hash)
        throw invalid_argument("secret key mismatch");

    // since xor (^) is assocative and commutative,
    //     + : trapdoor_union<X> -> trapdoor_union<X> -> trapdoor_union<X>
    // is also assocative and commutative.
    return trapdoor_union<X>(x.value_hash ^ y.value_hash, x.key_hash);
}


template <typename X>
auto insert(trapdoor<X> const & x, trapdoor_intersection<X> xs)
{
    if (x.key_hash != xs.key_hash)
        throw invalid_argument("mismatched secret keys");

    return trapdoor_intersection<X>(xs.value_hash ^ x.value, x.key_hash);
}

