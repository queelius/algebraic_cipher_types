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
 * 
 * 
 * 
 */


template <typename X>
struct trapdoor_concat: public trapdoor<kleene<X>>
{

}

template <typename X>
trapdoor_concat<X> concat(
    trapdoor<X> const & x,
    trapdoor<X> const & y)
{
    if (x.key_hash != y.key_hash)
        throw invalid_argument("mismatched secret keys")

    return trapdoor_concat<X>{x.value_hash ^ y.value_hash, x.key_hash};
};

template <typename X>
trapdoor_concat<X> concat(
    trapdoor_concat<X> const & x,
    trapdoor<X> const & y)
{
    if (x.key_hash != y.key_hash)
        throw invalid_argument("mismatched secret keys")

    return trapdoor_concat<X>{x.value_hash ^ y.value_hash, x.key_hash};
};

template <typename X>
trapdoor_concat<X> concat(
    trapdoor<X> const & x,
    trapdoor_concat<X> const & y)
{
    if (x.key_hash != y.key_hash)
        throw invalid_argument("mismatched secret keys")

    return trapdoor_concat<X>{x.value_hash ^ y.value_hash, x.key_hash};
};

/**
 * How to deal with associativity? concat(concat(x,y),z) = concat(x,concat(y,z)).
 * 
 * But same behavior as set. 
 * 
 */





