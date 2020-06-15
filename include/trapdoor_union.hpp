#include "trapdoor.hpp"
#include "trapdoor_set.hpp"

template <typename X>
struct power_set
{
    template <template <typename> typename Set>
    bool contains(Set<X> const&) { return true; }

};

template <typename X, template <typename> typename Set>
bool subset_eq(Set<X> const &, power_set<X> const&)
{
    return true;
}

template <typename X, template <typename> typename Set>
bool subset_eq(power_set<X> const &, Set<X> const &)
{
    return false;
}

struct empty_set
{
    template <typename X>
    bool contains(X const&) const { return false; }
};

 template <typename Set>
 bool subset_eq(empty_set const &, Set const &)
{
    return true;
}



template <typename X>
struct trapdoor_set: public trapdoor<power_set<X>>
{
};



/**
 * A trapdoor_union_expr<X> is a trapdoor<powerset<X>>.
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
