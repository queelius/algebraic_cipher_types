/**
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
/**
 * How to deal with associativity? concat(concat(x,y),z) = concat(x,concat(y,z)).
 * 
 * But same behavior as set. 
 * 
 *
 * 
 * 
 */

#include "trapdoor.hpp"

struct seq_of {};

template <typename X>
struct trapdoor_seq: public trapdoor<seq_of<trapdoor<X>>
{
    trapdoor_seq() : length(0), value_hash(0), key_hash(0) {};

    unsigned int length;
    unsigned int value_hash;
    unsigned int key_hash;
}

template <typename X>
auto make_empty_trapdoor_seq()
{
    return trapdoor_seq<X>();
}

template <typename X>
auto concat(
    trapdoor_seq<X> const & xs,
    trapdoor<X> const & x)
{
    if (!is_empty(xs) && x.key_hash != xs.key_hash)
        throw invalid_argument("mismatched secret keys")

    return trapdoor_seq<X>(
        xs.length + 1,
        hash(x.value_hash ^ xs.value_hash ^ xs.length),
        x.key_hash);
}

template <typename X>
auto concat(
    trapdoor<X> const & x,
    trapdoor<X> const & y)
{
    if (x.key_hash != y.key_hash)
        throw invalid_argument("mismatched secret keys")

    return concat(concat(make_empty_trapdoor_seq<X>(), x), y);
}

template <typename X>
auto length(trapdoor_seq<X> const & xs)
{
    return xs.length;
}

template <typename X>
auto is_empty(trapdoor_seq<X> const & xs)
{
    // if length information is deleted, then we can still use the identity
    // that rep(empty_seq) == 0.
    return xs.length == 0;
}
