#pragma once

#include <memory>
using std::shared_ptr;

/**
 * We apply type-erasure to allow any object that models the concept of
 * a cipher value type to be wrapped into cipher<X>, whether a Boolean cipher or
 * a cipher map of type A -> B. If the type of X is too revealing we may just
 * type-erase it to cipher<hidden_value_type>.
 *
 * NOTE: i think we went in a diff. direction, template specializations.
 * so, we have struct cipher{} and then
 * cipher<bool>, cipher<vector<X>>, cipher<function<Y(X)>>, and so on.
 */

struct hidden_tag_type {};

template <typename X = hidden_value_type>
struct cipher
{
    using value_type = hidden_t;

    template <typename Cipher>
    cipher(Cipher const & x) : cipher_(make_shared<model<Cipher>>{x})
    {
    }       

    struct concept
    {
        virtual size_t hash() const = 0;
        virtual size_t key_hash() const = 0;
    };

    template <typename Cipher>
    struct model final: public concept
    {
        size_t hash() const { return hash(cipher_); }
        size_t key_hash() const { return key_hash(cipher_); }

        Cipher cipher_;
    };

    shared_ptr<concept const> cipher_;
}

