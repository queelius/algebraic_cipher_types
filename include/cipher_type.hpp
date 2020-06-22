#pragma once

#include <memory>

struct hidden_value_type {};

template <typename X = hidden_value_type>
struct cipher
{
    using value_type = hidden_t;

    template <typename Cipher>
    cipher(Cipher const & x)
    {
    }       

    struct concept
    {
        virtual unsigned int hash() const = 0;

        // We denote a cipher<X> value generated
        // with a secret s such that hash(s) = H
        // by cipher<X,H>.
        //
        // cipher values generated with different
        // secrets are not the same (dependent
        // type) and functions of cipher values
        // vary over type but do not typically vary
        // over secrets.
        //
        // For example, functions of type
        //     cipher<X,H> -> cipher<Y,H> -> cipher<Z,H>
        // are typical but functions of type
        //     cipher<X,H1> -> cipher<Y,H2> -> cipher<Z,H3>
        // are atypical.
        //
        // There may be one excpetion, which takes
        // the form of a functor, or a homomorphism.
        //
        // Suppose we have a functor, or homomorphism
        //     f : cipher<X,H1> -> cipher<X,H2>
        // where H1 != H2 and
        //     D[H] : cipher<X,H> -> X
        // where D[H] is just a decoder that decodes
        // cipher<X,H> to X. (Presumably, D[H] is
        // a partial application of a decoder that
        // requires a secret key as an argument.)
        //
        // Two cipher<X> values x and y with
        // different secret hashes H1 and H2
        // are equivalent if
        //     dec[H1] (g x) == dec[H2] (f g) (f y),
        // where g : cipher<X,H1> -> cipher<Y,H1>
        // and (f g) : cipher<X,H2> - cipher<Y,H2>.
        //
        // Note that the equivalence may be *false*
        // since infinitely many secrets map to the
        // same finite hash.
        //
        // The reason we store the hash of the secret,
        // instead of the secret, is plainly obvious:
        // A cipher<X,H> would not be a cipher if it
        // stored the secret key to decode it.
        //
        // However, storing a hash of the secret
        // may also be too revealing, in which case
        // we may "type-erased" cipher<X,H> to cipher<X>.
        //
        // Note #1:
        //
        // We could make the key hash a part of the
        // template parameter, as suggested by cipher<X,H>,
        // to enable strong typing. However, this is
        // arguably too restrictive since the untrusted
        // system and the trusted system would need to
        // agree, at compile time, on the key hash.
        //
        // Note #2: 
        //
        // Type disclore, cipher<X>, may also be
        // too restrictive or too revealing, so the default
        // cipher type is cipher<hidden_type>.
        virtual unsigned int key_hash() const = 0;
    };

    template <typename Cipher>
    struct model
    {
        unsigned int hash() const { return hash(cipher_); }
        unsigned int key_hash() const { return key_hash(cipher_); }

        Cipher cipher_;
    };

    shared_ptr<concept const> cipher_;
}

