#include "trapdoor.hpp"


/**
 * If X = {a,b,c}, then 2^X = {{},{a},{b},{c},{a,b},{a,c},{b,c},{a,b,c}}.
 * 
 * A Boolean index over X is a Boolean algebra over 2^X with {}=0 and X=1
 * with the normal set operations. This is what a lot of prior work was over.
 * 
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
struct set_of {};

template <typename X>
struct trapdoor_set: public trapdoor<set_of<trapdoor<X>>>
{
    using value_type = X;

    trapdoor_set() :
        cardinality(0),
        value_hash(0),
        key_hash(0)
    {
        // makes the empty set
    }

    trapdoor_set(trapdoor_set const & copy) :
        cardinality(copy.cardinality),
        value_hash(copy.value_hash),
        key_hash(copy.key_hash) {}

    unsigned int cardinality;
    unsigned int value_hash;
    unsigned int key_hash;
};

template <typename X>
auto make_empty_trapdoor_set()
{
    return trapdoor_set<X>();
}

/**
 * The disjoint union operation is a partial function that is only defined
 * when the argument sets are disjoint (it is a dependent type). If they are
 * not disjoint, the operation has undefined behavior.
 */
template <typename X>
auto disjoint_union(
    trapdoor_set<X> const & x,
    trapdoor_set<X> const & y)
{
    if (x.key_hash != y.key_hash)
        throw invalid_argument("secret key mismatch");

    // since xor (^) is assocative and commutative,
    //     + : trapdoor_disjoint_union<X> -> trapdoor_disjoint_union<X> -> trapdoor_disjoint_union<X>
    // is also assocative and commutative.
    return trapdoor_set<X>(
        x.cardinality + y.cardinality,
        x.value_hash ^ y.value_hash,
        x.key_hash);
}

/**
 * disjoint_insert is a partial function that is only defined when {x} and xs are disjoint.
 */
template <typename X>
auto disjoint_insert(trapdoor<X> const & x, trapdoor_set<X> xs)
{
    if (x.key_hash != xs.key_hash)
        throw invalid_argument("secret key mismatch");

    return trapdoor_set<X>(
        xs.cardinality + 1,
        xs.value_hash ^ x.value,
        xs.key_hash);
}

/**
 * remove is a partial function that is only defined when x in xs.
 */
template <typename X>
auto remove(
    trapdoor<X> const & x,
    trapdoor_set<X> const & xs)
{
    return trapdoor_set<X>(
        xs.cardinality - 1,
        xs.value_hash ^ x.value,
        xs.key_hash);
}

template <typename X>
bool is_empty(trapdoor_set<X> const & xs)
{
    // do not technically need to track cardinality; the empty set has a
    // representation given by all zeros. However, other trapdoor sets may
    // also (falsely) evaluate to this.
    //
    // The *kernel* of an approximate value type is given by the following:
    //
    //     template <typename X>
    //     auto ker(approx_value<X> type) :=
    //         { x in X | identity<approx_type>::value == approx_type<X>(x) }.
    return xs.cardinality == 0;
}

template <typename X>
bool cardinality(trapdoor_set<X> const & xs)
{
    return xs.cardinality;
}

template <typename X>
bool is_singleton_element(trapdoor<X> const & x, trapdoor_set<X> const & xs)
{
    return
        xs.cardinality == 1 &&
        x.key_hash == xs.key_hash &&
        x.value_hash == xs.value_hash;
}

