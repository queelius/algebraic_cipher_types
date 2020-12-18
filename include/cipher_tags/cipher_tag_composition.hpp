#pragma once
#include "cipher_tag.hpp"
/**
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
 */

/**
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
 */

namespace alex::cipher
{
    /**
     * When we compose cipher tags, e.g., product<t,u> where
     * fpr(t) = 2^(-n) and fpr(u) = 2^(-m), then we have a
     * higher-order random approximation.
     */

    template <typename A, typename B>
    struct cipher_tag_product
    {
        // use intervals and a higher-order approximate
        // type to provide a minspan for the fpr.
        auto operator==(cipher_tag_product const & rhs) const
        {
            // this should evaluate to fpr(a,rhs.a) * fpr(b,rhs.b).
            // if a,b are second-order models, and so is rhs's a and b,
            // then so is cipher_tag_product.
            return a == rhs.a && b == rhs.b;
        }

        auto operator!=(cipher_tag_product const & rhs) const
        {
            return !(*this == rhs);
        }

        // we have the basic primitive cipher
        // tags, but we may compose cipher
        // tags by hashing and xoring the
        // primitives. done recursively,
        // just a few primitive cipher tags can
        // cover many types.

        // do we want to do the list this way?
        // say we have a list of type X.
        // then, we could say this is a product
        // () * X * X^2 * X^3 * ...,
        // i.e., a recursive type, but in this
        // case it probably makes more sense
        // to say 'list X'. same for 'set X'?
        // but, this means we should be labeling
        // things as abstract data types? there
        // are many ways to model a set, and
        // a cipher map for instance will work
        // on their hashes to determine if, say
        // it has a set as a member or whatever.
        operator cipher_tag() const
        {
            cipher_tag h;
            h.s = cipher_of_secret(a);
            h.value  = std::hash("(product ");
            h.value ^= std::hash(a);
            h.value ^= std::hash(b);
            h.value ^= std::hash(")");
            return h;
        }

        A a;
        B b;
    };

    template <>
    struct deserialize<cipher_tag_product<cipher_tag,cipher_tag>>
    {
        template <typename I>
        pair<optional<cipher_tag_product<cipher_tag,cipher_tag>>,I>
        operator()(I begin, I end) const
        {
            if (begin == end)
                return make_pair(nullopt,begin);
            cipher_tag_product tag;
            tag.s = (cipher_tag::cipher_secret_type)strtoull(*begin++,nullptr,10);
            if (begin == end)
                return make_pair(nullopt,begin);

            tag.value = (cipher_tag::value_type)strtoull(*begin++,nullptr,10);

            return make_pair(tag,begin);
        }
    };



    template <typename T>
    struct serialize {};

    template <>
    struct serialize<cipher_tag_product>
    {
        template <typename O>
        O operator()(O out) const
        {
            *o++ = "(";
            *o++ =     "product";
            *o++ =     "(";
            *o++ =         "cipher_secret ";
            *o++ =         to_string(cipher_of_secret(a));
            *o++ =     ")";
            *o++ =     to_string(hash(a));
            *o++ =     to_string(hash(b));
            *o++ = ")";
            return o;
        }
    };

    template <typename A, typename B>
    struct cipher_tag_sum
    {
        // use intervals and a higher-order approximate
        // type to provide a minspan for the fpr.
        auto operator==(cipher_tag_sum const & rhs) const
        {
            // this should evaluate to fpr(a,rhs.a) * fpr(b,rhs.b).
            // if a,b are second-order models, and so is rhs's a and b,
            // then so is cipher_tag_sum.
            return a == rhs.a && b == rhs.b;
        }

        auto operator!=(cipher_tag_sum const & rhs) const
        {
            return !(*this == rhs);
        }

        A a;
        B b;
    };

    // the above does not cipher the fact that it is a product of two cipher
    // types. however, if we hash the final serialization, we can accomplish
    // this and then we'd get a single cipher_tag.


    // models the concept of a function of type
    //     A::value_type -> B::value_type
    template <typename A, typename B>
    struct cipher_tag_exponential
    {
        using domain_type = typename A::value_type;
        using codomain_type = typename B::value_type;

        // use intervals and a higher-order approximate
        // type to provide a minspan for the fpr.
        auto operator==(cipher_tag_exponential const & rhs) const
        {
            // this should evaluate to fpr(a,rhs.a) * fpr(b,rhs.b).
            // if a,b are second-order models, and so is rhs's a and b,
            // then so is cipher_tag_sum.
            return a == rhs.a && b == rhs.b;
        }

        auto operator!=(cipher_tag_exponential const & rhs) const
        {
            return !(*this == rhs);
        }

        operator cipher_tag() const
        {
            cipher_tag h;
            h.s = cipher_of_secret(a);
            h.value  = std::hash("(exponential ");
            h.value ^= std::hash(a);
            h.value ^= std::hash(b);
            h.value ^= std::hash(")");
            return h;
        }

        A a;
        B b;
    };

    template <>
    struct serialize<cipher_tag_exponential>
    {
        template <typename O>
        O operator()(O out) const
        {
            *o++ = "(";
            *o++ =     "exponential";
            *o++ =     "(";
            *o++ =         "cipher_secret ";
            *o++ =         to_string(cipher_of_secret(a));
            *o++ =     ")";
            *o++ =     to_string(hash(a));
            *o++ =     to_string(hash(b));
            *o++ = ")";
            return o;
        }
    };

}