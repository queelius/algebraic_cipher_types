#pragma once

/**
 * We have a simple type system.
 * 
 * Type info (plaintext) is converted
 * to cipher type info.
 * 
 * Type info tells us how to interpret
 * the bits a value represents, e.g., a
 * value of type bool has a false value
 * given by all zeros and any other
 * is true.
 * 
 * First, we map a primitive type's tag
 * to a cipher tag (e.g., hash w/secret).
 * This is sufficient to do approximate
 * type checking, and if the secret is
 * known then we may be able to reconstruct
 * the type, assuming its not a trapdoor
 * type.
 * 
 * We compose these primitive types using
 * operations like sum type and product
 * type.
 * 
 * Sum type X+Y is reprented as
 *     (+ (cipher_type_info X) (cipher_type_info Y))
 * where (cipher_type_info X) is just, say
 * a hash of the type info for X.
 * 
 * Uses S-expressions to allow for easy
 * recursive types. For instance,
 * X+(Y*Z) has
 *     (+ (cipher_type_info X)
 *        (* (cipher_type_info Y) (cipher_type_info Z))
 * We don't cipher + and *, only the primitives,
 * since to check that, say, a function is accepting
 * only arguments of type cipher_type_info X or
 * cipher_type_info Y, we need to know that its a
 * sum type.
 * 
 * Note that if a cipher of (+ X Y) is used, such that
 * X and Y are cominged (not independent), then we
 * may use hash(X) | hash(Y) as the type instead,
 * and for a cipher of (* X Y), a pair, we may
 * use hash(X) ^ hash(Y) instead. This is reasonable
 * since we've ciphered the pairs, i.e.,
 *     cipher(X + Y) instead of cipher(X) + cipher(Y).
 * 
 * We can also do the same for more complex types,
 * or even custom algebraic data types, e.g.,
 * instead of type (cipher X)*, a list of (cipher X)
 * we may have a type cipher(X*) with a cipher info
 * hash(cipher(X*)) instead of list(cipher(X)).
 * 
 * 
 * 
 *
 * 
 */


#include <string>
#include <cmath>
#include <optional>
#include <utility>
using std::pair;
using std::string;
using std::size_t;
using std::make_pair;
using std::nullopt;
using std::optional;
using std::strtoull;
using std::to_string;

namespace alex::cipher
{
    struct cipher_tag
    {
        using value_type = size_t;
        using cipher_secret_type = size_t;

        value_type value;
        cipher_secret_type s;

        /**
         * Suppose we have two approximate cipher tags a and b.
         * The false negative rate on tag equality is zero (positive aset).
         * The false positive rate on tag equality is
         *
         *           2^(-k) 2^(-l) (2 - p) (1 - q) + 2^(-k) q (1 - p)
         *     fpr = ------------------------------------------------,
         *                      1 - p q
         * 
         * where k and l are the respective bit lengths of the tag values of a and b,
         * p is the prior probability that a and b truly have the same type, and
         * q is the prior probability that the secrets used to encode a and b are
         * the same.
         * 
         * Generally, when the queries are generated from a trusted system, we assume
         * that q = 1, in which case
         * 
         *     fpr = 2^(-k).
         */
        auto operator==(cipher_tag const & rhs) const
        {
            return rhs.value == value && rhs.s == s;
            //return approximate<2,bool>{
            //    std::pow(2.0,-sizeof(value) * 8),
            //    rhs.value == value && rhs.s == s};
        }

        auto operator!=(cipher_tag const & rhs) const
        {
            return !(*this == rhs);
        }
    };        


    template <typename T>
    struct deserialize {};

    template <>
    struct deserialize<cipher_tag>
    {
        template <typename I>
        pair<optional<cipher_tag>,I> operator()(I begin, I end) const
        {
            if (begin == end)
                return make_pair(nullopt,begin);
            cipher_tag tag;
            tag.s = (cipher_tag::cipher_secret_type)strtoull(*begin++,nullptr,10);
            if (begin == end)
                return make_pair(nullopt,begin);

            tag.value = (cipher_tag::value_type)strtoull(*begin++,nullptr,10);

            return make_pair(tag,begin);
        }
    };

}