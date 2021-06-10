#pragma once

/**
 * We have a simple type system.
 * 
 * Type info (plaintext) is converted
 * to cipher type info.
 * 
 * Type info tells us what the bits of
 * a value represents, e.g., a value
 * of type bool has a false value
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


#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <string_view>
#include <optional>
#include "cipher_tag.hpp"

using std::string;
using std::move;
using std::string_view;
using std::hash;
using std::map;
using std::optional;
using std::istream;
using std::ostream;
using std::nullopt;

namespace alex::cipher
{
    // handles the management of assigning ciphertext to type info, whether
    // plaintext or cipher value types. in either way, the value that the bits
    // represent is obscured.
    //
    // given the secret, one can query type information of values. if the secret
    // is not known, then only the type equality query is provided, i.e.,
    // whatever types two values represent, we can determine if they have the
    // same type.
    template <size_t MagicBits>
    struct cipher_tag_registry
    {
        using trapdoor_type = size_t;
        using cipher_type = size_t;
        using label_type = string;
        using secret_type = string_view;

        // approximate type on
        //     == : cipher_type_info -> cipher_type_info -> bool
        struct cipher_tag_info
        {
            cipher_tag value;
            cipher_type_registry const & reg;

            bool operator==(cipher_type_info const & rhs) const
            {
                // since secrets may be different, this is a positive-negative
                // boolean where both false negatives and false positives
                // may occur.
                return rhs.value == value;
            }

            bool operator!=(cipher_type_info const & rhs) const
            {
                return !(*this == rhs);
            }

            // convert a cipher type denoted by h to its plaintext type.
            optional<string> plaintext(secret_type s) const
            {
                return reg.plaintext(value, s);
            }

            template <typename T>
            bool is_type(secret_type s) const
            {
                return reg.is_type<T>(value, s);
            }

            // determine if ciphertype denotes a cipher type.
            // returns a second-order positive approximate Boolean with a 
            // false positive rate 2^-H where H := sizeof(hash_type).
            bool is_any_type(secret_type s) const
            {
                return reg.is_any_type(value, s);
            }
        };        

        static auto metadata()
        {
            struct 
            {
                // if this is false, then no information (other than equality
                // of type) may be queried about a value.
                static auto queryable_type_info() { return true; }

                // if we stored the actual secret, then any cipher values
                // would be convertible to plaintext values. as a compromise,
                // we store a hash of the secret, which faclilitates strong
                // type checking.
                static auto cipher_of_secret() { return true; }

                // the version of the cipher type info
                static auto version() { return static_cast<size_t>(1); }

                // the header. this may be revealed in the type system too,
                // so that there can be common agreement on the algorithms.
                static auto header() { return "cipher_type_registry"; }

                // a small customization point so that even if the same secret
                // is in use, different ciphers of the same types may be
                // generated by varying magicbits().
                static auto magic_bits() { return MagicBits; }
            } ret;
            return ret;
        }

        cipher_type_registry(trapdoor_type cipher_of_secret) : secret_hash_(cipher_of_secret) {}
        cipher_type_registry() = default;
        cipher_type_registry(cipher_type_registry const &) = default;
        cipher_type_registry(secret_type secret) : secret_hash_(cipher_of_secret(secret)) {}
        cipher_type_registry(istream & in) { deserialize(in); }

        static trapdoor_type cipher_of_secret(secret_type secret)
        {
            return hash<string_view>{}(secret) ^ MagicBits;
        }

        template <typename T>
        bool insert(string type, secret_type s)
        {
            if (!is_secret(s))
                return false;

            tags_[move(type)] = cipher(typeid(T).hash_code(), s);
            return true;
        }

        bool insert(string type, secret_type s)
        {
            if (!is_secret(s))
                return false;

            tags_[move(type)] = cipher(type, s);
            return true;
        }

        // second-order positive approximate Boolean.
        // False positive with probability 2^-H where H := sizeof(hash_type).
        bool is_secret(secret_type s) const
        {
            return cipher_of_secret() == cipher_of_secret(s);
        }

        template <typename T>
        static cipher_type cipher(T const & x, secret_type s)
        {
            return hash<T>{}(x) ^ hash<string_view>{}(s) ^ MagicBits;
        }

        struct const_iterator
        {
            map<string, cipher_type>::const_iterator cur;
            cipher_type_registry const & r;

            bool operator==(const_iterator const & rhs) const
            {
                return cur == rhs.cur;
            }

            bool operator!=(const_iterator const & rhs) const
            {
                return cur != rhs.cur;
            }

            const_iterator & operator++() { ++cur; return *this; }

            auto operator*() const { return r.type_info(cur->second); }                
        };

        auto begin() const { return const_iterator{tags_.begin(),*this}; }
        auto end() const { return const_iterator{tags_.end(),*this}; }
        auto size() const { return tags_.size(); }

        void serialize(ostream& o) const
        {
            o << metadata().header() << "\n";
            o << metadata().version() << "\n";
            o << cipher_of_secret() << "\n";
            o << size() << "\n";
            for (auto const & t : *this)
                o << t.first << "\t" << t.second << "\n";
        }

        bool deserialize(istream & is)
        {
            string hdr;
            is >> hdr;
            if (hdr != metadata().header())
                return false;

            size_t ver;
            is >> ver;
            if (ver != metadata().version())
                return false;

            is >> secret_hash_;
            size_t n;
            is >> n;
            for (size_t i = 0; i < n; ++i)
            {
                string type_label;
                is >> type_label;
                is >> tags_[type_label];
            }
            return true;
        }

        cipher_type_info type_info(cipher_type t) const
        {
            return cipher_type_info{t,*this};
        }

        trapdoor_type cipher_of_secret() const { return secret_hash_; }

        // convert a cipher type denoted by h to its plaintext type.
        optional<string> plaintext(cipher_type t, secret_type s) const
        {
            if (!is_secret(s))
                return nullopt;

            for (auto const & x : tags_)
            {
                if (x.second == t)
                    return x.first;
            }
            return nullopt;
        }

        template <typename T>
        T as_type(cipher_value<T> x, secret_type s) const
        {
            return 
        }


        template <typename T>
        bool is_type(cipher_type t, secret_type s) const
        {
            return cipher(typeid(T).hash_code(), s) == t;
        }

        // determine if ciphertype denotes a cipher type.
        // returns a second-order positive approximate Boolean with a 
        // false positive rate 2^-H where H := sizeof(hash_type).
        bool is_any_type(cipher_type t, secret_type s) const
        {
            if (!is_secret(s))
                return false;

            for (auto const & x : tags_)
            {
                if (x.second == t)
                    return true;
            }
            return false;

        }

    private:
        trapdoor_type secret_hash_;
        map<string, cipher_type> tags_;
    };
}