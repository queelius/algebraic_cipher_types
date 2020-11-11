#pragma once

#include <memory>
using std::shared_ptr;


// type-erase model of the the concept
// template<>
// class cipher<cipher<bool>,cipher<T>>

// cipher if : cipher bool -> T -> T
// this means that 'if' is a cipher map
// that accepts a cipher bool and returns
// a value of type T.
//
//
// suppose the original if expression was
// 
// let E := (if expr -1 1)
// then, E 1 = -1
// and   E 0 = 1.
//
// (cipher E) maps E to a cipher
// expression in which the code
// (if expr -1 1) is obscured to
// cipher E := (if expr a' b')
// such that (cipher E) 1 = -1
//       and (cipher E) 0 = 1.
// now, we also wish to pass
// a cipher bool as the expr
// instead of a bool.
// 
// 
// 

// let's also type-erase any cipher that
// models a cipher bool to cipher<bool>.

struct cipher_type {};

template <typename A = cipher_type, typename B = cipher_type>
class cipher_if
{
public:
    using input_type = cipher<bool>;
    using output_true_type = A;
    using output_false_type = B;

    auto operator()(cipher<bool> e) const
    {
        // f is a cipher map on cipher[bool]
        return if_->eval(e);
    }

    // CipherMap models the concept of
    // a function of type
    //     cipher<bool> -> A + B
    template <typename CipherMap>
    cipher_if(CipherMap if) : make_shared<CipherMap>{if} {}

private:
    struct concept
    {
        virtual T eval(cipher<bool> e) const = 0;
    };

    shared_ptr<concept const> if_;

    template <typename CipherMap>
    struct model final : concept
    {
        CipherMap if_;

        cipher<T> eval(cipher<bool> e) const
        {
            return if_(e);
        }
    };
};




/**
 * cipher<bool> models the concept of an
 * approximate cipher Boolean.
 * 
 * it does not support equality operations;
 * rather, a secret must be known to decode
 * to a plaintext bool to perform such queries.
 */

/**
 * process;
 * 
 * specify input type X
 * specify output type Y
 * 
 * specify (cipher) representations for X (cipher X)
 * specify (cipher) representations for Y (cipher Y)
 * 
 * define function of f: X -> Y
 * 
 * for each x in X:
 *     y := f(x)
 *     [y'] := cipher(y)
 *     ...
 * 
 * 
 * can use a canonical representation of a body
 * e.g., renaming variables to some canonical form,
 * renaming function names to some canoncial form,
 * etc., and then hashing it, to quickly check that
 * two ciphers (say, a cipher of a function) model
 * the same thing. same for types. in this way,
 * we can verify that two, say, cipher bools are
 * of the same type, i.e., same ciphers for true and
 * false.
 * 
 * 
 */


struct cipher
{
    
}

template <>
class cipher<bool>
{
public:
    using plain_value_type = bool;

    // make this a separate class?
    using meta_info_type = map<string, variant<string,size_t,cipher_type>>;

    auto fpr() const { return bool_->fpr(); }
    auto fnr() const { return bool_->fnr(); }
    auto size() const { return bool_->size(); }
    auto meta_info() const { return bool_->meta_info(); }
    auto try_convert(string_view secret) const
    {
        return bool_->try_convert(secret);
    }

private:
    struct concept
    {
        virtual double fnr() const = 0;
        virtual double fpr() const = 0;
        virtual size_t size() const = 0;

        // what should meta_info return?
        // instead of string, return a map?
        virtual meta_info_type meta_info() const = 0;
        virtual optional<bool> try_convert(string_view) const = 0;
    };

    shared_ptr<concept const> bool_;

    template <typename C>
    struct model final : concept
    {
        size_t size() const { return size(c); }
        double fnr() const { return fnr(c); }
        double fpr() const { return fpr(c); }
        string meta_info() const { return meta_info(c); }
        optional<bool> try_convert(string_view secret) const
        {
            return try_convert(c,secret);
        }

        C c;
    };
};

auto fnr(cipher<bool> const & x)
{
    return x.fnr();
}

auto size(cipher<bool> const & x)
{
    return x.size();
}

auto fpr(cipher<bool> const & x)
{
    return x.fpr();
}

auto meta_info(cipher<bool> const & x)
{
    return x.meta_info();
}

auto convert_to(cipher<bool> const & x, string_view secret)
{
    return x.try_convert(secret);
}