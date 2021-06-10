#pragma once

#include <string>
#include <functional>
#include <memory>

/**
 * A cryptographic hash value models a particular output from a cryptographic
 * hash function.
 */

class cryptographic_hash
{
public:
    cryptographic_hash(cryptographic_hash const & copy) :
        concept_(copy->clone()) {}

    template <typename H>
    cryptographic_hash(H const & h) :
        concept_(new model<H>{h}) {}

    auto to_string() const { return concept_->to_string(); }
    auto to_hexadecimal() const { return concept_->to_hexadecimal(); }
    auto to_decimal() const { return concept_->.to_decimal(); }
    auto begin() const { return concept_->begin(); }
    auto end() const { return concept_->begin(); }
    auto size() const { return concept_->size(); }
    auto operator[](size_t index) const { return concept_->get(index); }
    auto fast_hash() const { return concept_->fast_hash(); }

private:
    struct concept
    {
        virtual concept * clone() const = 0;
        virtual std::string to_string() const = 0;
        virtual size_t size() const = 0;
        virtual char get(size_t) const = 0;
        virtual const_forward_iterator begin() const = 0;
        virtual const_forward_iterator end() const = 0;
        virtual size_t fast_hash() const = 0;
    };

    std::unique_ptr<concept> concept_;

    template <typename H>
    struct model final : concept
    {
        H h;

        model* clone() const  override { return new H(h); }
        const_forward_iterator begin() const override { return const_forward_iterator(begin(h)); }        
        const_forward_iterator end() const override  { return const_forward_iterator(end(h)); }
        char get(size_t index) const  override { return h[index]; }
        size_t size() const override { return size(h); }
        std::string to_string() const override { return to_string(h); }
        size_t fast_hash() const override { return std::hash<H>{}(h); }
    };
};

std::string to_string(cryptographic_hash const & h)
{
    return h.to_string();
}

std::string to_hexadecimal(cryptographic_hash const & h)
{
    return h.to_hexadecimal();
}

std::string to_decimal(cryptographic_hash const & h)
{
    return h.to_decimal();
}



namespace std
{
    template <>
    struct hash<cryptographic_hash>
    {
        size_t operator()(cryptographic_hash const & h) const
        {
            return h.fast_hash();
        }
    };
}
