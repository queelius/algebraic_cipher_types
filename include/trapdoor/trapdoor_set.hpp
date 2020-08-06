#include "trapdoor.hpp"

/**
 * Consider the Boolean algebra
 *     A := (P(X*), and, or, complement, {}, X*)
 * where X is the alphabet and X* is the
 * free semigroup on X which is closed under
 * concatenation,
 *     : : X* -> X* -> X*.
 * 
 * For example, if
 *     X = {a,b}
 * then
 *     X* = {^, a, b, aa, ab,
 *           ba, bb, aaa, aab, ... }
 * and P(X*) is the power set of X*,
 *     P(X*) = {{}, ^, {a}, {b},
 *              {aa}, {a,aa}, {a,bb}, ... }.
 * 
 * Consider the Boolean algebra
 *     B := ({0,1}^n, &, |, ~, 0^n, 1^n)
 * and suppose we have a homomorphism
 *     F : A -> B
 * defined in the following way.
 * 
 * First, we have a cryptographic hash
 * function
 *     hash : X* -> {0,1}^n
 * that a priori uniformly distributes
 * over {0,1}^n, i.e., each X* maps to
 * any element in the {0,1}^n with
 * probability 2^(-n).
 * 
 * Then, homomorphism F maps strings
 * in X* to bit strings in {0,1}^n
 * by applying the hash function
 * to the input concatenated with
 * a secret s,
 *     F a := hash(a s).
 * 
 * Note #1: Later, we generalize this
 * to mapping each a in X* to multiple
 * elements in {0,1}^n proportional to
 * 1/P[a].
 * 
 * Observe that F is one-way, i.e., there
 * is no homomorphism G such that
 *     F G B = A.
 * 
 * Theorem:
 * The morphism F defined as
 * 
 *     X*         := hash(a # s)
 *     and        := &
 *     or         := |
 *     complement := ~
 *     {}         := 0^n
 *     X*         := 1^n.
 * 
 * is a homomorphism.

 * Proof:
 * 
 * F ({} or A) = F {} | F A = 0^n | F A = F A
 * 
 * F ({} and A) = F {} & F A = 0^n & F A = 0^n
 * 
 * We wish to prove F (A or B) = F A + F B.
 * F ({a} or {b}) = F {a} | F {b}
 * 
 * We wish to prove F (A and B) = F A + F B.
 * F ({a} and {b}) = F {a} & F {b}
 * 
 * We wish to prove F (complement A) = ~ F A.
 * F (complement {a}) = ~ F {a} 
 * 
 * qed
 * 
 * Since multiple elements in X* map to the same
 * element in {0,1}^n, it is a homomorphism rather
 * than an isomorphism.
 * 
 * What is the probability that two unique elements
 * in X* map to the same element in {0,1}^n? That is
 * to say, what is the probability of collision?
 * Since F uniformly distributes over {0,1}^n, it
 * is just
 * 
 *     P[x and y collide] = 2^(-n).
 * 
 * By the law of probability, therefore, the
 * probability that they do not collide is just
 * 
 *     P[x and y do not collide] = 1 - 2^(-n).
 * 
 * Next, we define relations on sets. Set
 * membership relation has a characteristic
 * function
 *    in : X -> 2^X -> bool
 * which we define as
 *    F in a b := a & b == a.
 * 
 * The subset relation has a predicate
 *    subset : 2^X -> 2^X -> bool
 * which we define as
 *    F subset a b := a & b == a,
 * just as with the characteristic function,
 * although they have different probabilistic
 * features.
 *
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
struct trapdoor_set
{
    using value_type = X;

    trapdoor_set() :
        value_hash(0),
        key_hash(0)
    {
        // makes the empty set
    }

    trapdoor_set(trapdoor_set const & copy) = default;

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

