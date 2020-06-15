


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
struct concat {};

template <typename X>
struct identity {};

struct search_key
{
    std::string value;
};

/**
 * A trapdoor of X is a one-way transformation of values of type X
 * to trapdoor<X>. Let the one-way transformation be denoted by
 *     make_trapdoor : {0,1}^* -> X -> trapdoor<X>,
 * where the first argumen is a secret key.
 * 
 * Then, the partial application
 *     T(x) := \x -> make_trapdoor(s,x)
 * is of type X -> trapdoor<X> where s is the secret.
 * 
 * T is one-way in two independent senses:
 * 
 *     (1) T is easy to compute, but its "inverse" U : trapdoor<X> -> 2^X
 *         is (generally) hard.
 * 
 *     (2) Since T is (generally) non-invertible, U(t) := { x in X | T(x) = t }
 *         and thus each t in trapdoor<X> may map to multiple values in X.
 *
 *         In many cases, such as when X is a the free semigroup of a finite alphabet,
 *         U(t) is countably infinite for any t in trapdoor<X>. Thus, revealing
 *         which values U(t) maps to may not be very informative.
 */
template <typename X>
struct trapdoor
{
    using value_type = X;

    trapdoor() : {}

    trapdoor(X const & x, string_view k)
    {
        key_hash = hash(k);
        value_hash = hash(x);
        value_hash ^= key_hash;
    }

    unsigned int value_hash;

    // the key hash is a hash of the secret key,
    // which faciliates a form of dynamic type checking.
    unsigned int key_hash;
};

template <typename X>
trapdoor<X> make_trapdoor(
    trapdoor<X> const & x,
    string_view k)
{
    return trapdoor<X>(x,k);
}

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
