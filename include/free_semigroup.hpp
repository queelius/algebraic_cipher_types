


/**
 * If X = {a,b,c}
 * Then 2^X = {{},{a},{b},{c},{a,b},{a,c},{b,c},{a,b,c}}.
 * 
 * A Boolean index over X is a Boolean algebra over 2^X with {}=0 and X=1
 * with the normal set operations. This is what a lot of prior work was over.
 * 
 * Now, we want to generalize.
 * 
 * If we say that X is an alphabet, then X* is a language over the alphabet.
 * 
 * X* = {^,a,b,c,aa,ab,ac,ba,bb,bc,ca,cb,cc,aaa,aab,aac,aba,abb,abc,aca,acb,acc,...}
 * 
 * Now, suppose we have a concatenation operator # : (X*,X*)->X*. This is an associative
 * non-commutative operator:
 *     (u#v)#t = u#(v#t)
 * and
 *     u#v != v#u
 * 
 * The null string ^ is the identity element:
 *     u#^ = ^#u = u
 * 
 * Let X*(k) = {x in X* | |x| = k}.

 * 
 * 
 * 
 * 
 * 
 */

#include <string>
#include <string_view>
#include <optional>

using std::string;
using std::string_view;
using std::optional;

unsigned int hash(string_view x)
{
    return 0;
}



template <typename X>
struct trapdoor_intersection
{
    using value_type = X;

    trapdoor_intersection() : value_hash(0), key_hash(0) {}

    trapdoor_intersection(trapdoor_intersection const & copy) :
        value_hash(copy.value_hash, copy.key_hash) {}

    unsigned int value_hash;
    unsigned int key_hash;
};

template <typename X>
struct trapdoor_empty_set
{
    // type-erased version of trapdoor sets
    // a trapdoor_set<X> is itself a trapdoor<powerset<X>>.
};


struct trapdoor_empty_set
{

};

// this models the empty set
template <typename X>
struct identity<trapdoor_union<X>>
{
    auto operator()() const
    {
        return trapdoor_union<X>();
    }
};

// this models the empty set
template <typename X>
struct identity<trapdoor_intersection<X>>
{
    auto operator()() const
    {
        return trapdoor_intersection<X>();
    }
};


template <typename X>
optional<trapdoor<X>> concat(
    trapdoor<X> const & x,
    trapdoor<X> const & y)
{
    if (x.key_hash != y.key_hash)
        return {};

    trapdoor<X> t;
    t.value_hash = x.value_hash ^ y.value_hash;

}

template <typename X>
optional<trapdoor_union<X>> operator+(
    trapdoor_union<X> const & x,
    trapdoor_union<X> const & y)
{
    if (x.key_hash != y.key_hash)
        return {};

    // since xor (^) is assocative and commutative,
    //     + : trapdoor_union<X> -> trapdoor_union<X> -> trapdoor_union<X>
    // is also assocative and commutative.
    return trapdoor_union<X>(x.value_hash ^ y.value_hash, x.key_hash);
}


template <typename X>
trapdoor_intersection<X> operator*(
    trapdoor_intersection<X> const & x,
    trapdoor_intersection<X> const & y)
{
    // a trapdoor of x of type X with respect to a secret k is different than
    // a trapdoor of x of type X with respect to a secret l. therefore, the
    // intersection of two such sets A and B respectively with secrets k and
    // l is the empty set, i.e., they do not overlap.
    if (x.key_hash != y.key_hash)
        return trapdoor_intersection<X>();

    // since xor (^) is assocative and commutative,
    //     * : trapdoor_intersection<X> -> trapdoor_intersection<X> -> trapdoor_intersection<X>
    // is also assocative and commutative.
    return trapdoor_intersection<X>(x.value_hash ^ y.value_hash, x.key_hash);

}


template <typename X>
auto insert(trapdoor<X> const & x, trapdoor_union<X> xs)
{
    if (x.key_hash != xs.key_hash)
        throw invalid_argument("mismatched secret keys");

    return trapdoor_union<X>(xs.value_hash ^ x.value, x.key_hash);
}


// the probability P[make_trapdoor(x) == make_trapdoor(y) | x != y],
// assuming a cryptographic hash function, is the probabiliity that
// they both hash to the same value, 1 / |unsigned int|.
template <typename X>
exponential_rate<unsigned int> error_rate(trapdoor<X> const & x)
{
    return exponential_rate<unsigned int>(sizeof(x.value_hash))
}

template <typename X>
bool operator==(trapdoor<X> const & x, trapdoor<X> const & y)
{
    return x.key_hash == y.key_hash && x.value_hash == y.value_hash;
}

template <typename X>
bool operator!=(trapdoor<X> const & x, trapdoor<X> const & y)
{
    return x.key_hash == y.key_hash && x.value_hash == y.value_hash;
}

template <typename X, typename Y>
bool operator==(trapdoor<X> const &, trapdoor<Y> const &)
{
    return false;
}

template <typename X1, typename X2>
auto operator+(
    trapdoor_union<X1> const & x,
    trapdoor_union<X2> const & y)
{
    // if X1 and X2 are different types, then the union is automatically
    // disjoint. Normally, this is fine, we could just represent their
    // union as normal, but 
    return trapdoor_disjoint_union<X1,X2>(
        x.value_hash, x.key_hash,
        y.value_hash, y.key_hash);
}


template <typename X1, typename X2>
auto operator*(
    trapdoor_intersection<X1> const & x,
    trapdoor_intersection<X2> const & y)
{
    // values of type X1 do not overlap with values of type X2 so return
    // the empty set.
    return trapdoor_empty_set{};
}

