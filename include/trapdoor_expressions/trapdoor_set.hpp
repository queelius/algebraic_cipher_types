/**
 * If X = {a,b,c}, then 2^X = {{},{a},{b},{c},{a,b},{a,c},{b,c},{a,b,c}}.
 * 
 * A Boolean index over X is a Boolean algebra over 2^X with {}=0 and X=1
 * with the normal set operations. This is what a lot of prior work was over.
 */


#include "trapdoor.hpp"

template <typename X>
struct power_set
{
    // should check to make sure Set models the concept of a set.
    template <template <typename> typename Set>
    bool contains(Set<X> const &) { return true; }
}

struct empty_set
{
    template <typename X>
    bool contains(X const &) { return false; }
}

struct universal_set
{
    template <typename X>
    bool contains(X const &) { return true; }
}

/**
 * Note that a type that models power_set<trapdoor<X>> is one in which
 * given a value A of this type, each element a in A is a trapdoor<X>
 * can be independently observed. This makes it possible to operate
 * on A as a normal set, with the exception that the mapping the trapdoors
 * to values may not be obvious (although given a history, or a set of
 * sets, frequency analysis or correlation analysis may reveal quite a bit).
 * 
 * The trapdoor_set<X> is one in which its not possible to do that.
 */
template <typename X>
struct trapdoor_set: public trapdoor<power_set<trapdoor<X>>>
{

};


template <typename X, template <typename> typename Set>
bool subset_eq(trapdoor_set<X> const &, trapdoor_power_set<X> const&)
{
    return true;
}

template <typename X, template <typename> typename Set>
bool subset_eq(trapdoor_power_set<X> const &, Set<X> const &)
{
    return false;
}

struct trapdoor_empty_set
{
    template <typename X>
    bool contains(X const&) const { return false; }
};

template <typename Set>
bool subset_eq(trapdoor_empty_set const &, Set const &)
{
    return true;
}
