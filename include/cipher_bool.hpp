#pragma once

/**
 * cipher<bool> models the concept of an
 * approximate cipher Boolean.
 * 
 * We know it is a cipher of a Boolean value,
 * 
 * It does not support equality operations;
 * rather, a secret must be known to decode
 * to a plaintext bool to perform such queries.
 */

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

        //         
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