#pragma once

template <typename F>
class cipher_map_map
{
public:
    using size_type = size_t;
    using input_type = typename F::input_type;
    using key_type = input_type;
    using output_element_type = F::output_type;
    using output_type = list_proxy;
    using hash_type = decltype(hash(declval(input_type)));

    struct key_forward_iterator
    {
        F & f_;
        hash_type h_;
        hash_type k_;

        auto & operator++()
        {
            h_ ^= hash(y_);
            y_ = f(h_);
            return *this;
        }

        bool operator==(forward_iterator const & rhs) const { return k_ == k; }
        bool operator!=(forward_iterator const & rhs) const { return k_ != k; }
        auto operator*() const { return y_; }
    };

    struct value_forward_iterator
    {
        F & f_;
        hash_type h_;
        hash_type k1_;
        hash_type k2_;

        auto & operator++()
        {
            h_ ^= hash(y_);
            y_ = f(h_);
            return *this;
        }

        bool operator==(forward_iterator const & rhs) const { return k_ == k; }
        bool operator!=(forward_iterator const & rhs) const { return k_ != k; }
        auto operator*() const { return y_; }
    };

    struct value_list_proxy
    {
        F & f_;
        hash_type k1_;
        hash_type k2_;

        constexpr auto sentinal() const
        {
            return hash_type(hash(0));
        }
    };

    struct key_list_proxy
    {
        F & f_;
        hash_type seed_key_;

        constexpr auto sentinal() const
        {
            return hash_type(hash(0));
        }
    };

    struct map_proxy
    {
        F & f_;
        hash_type k_;

        constexpr auto sentinal() const
        {
            return hash_type(hash(0));
        }

        auto begin() const
        {
            return key_forward_iterator{f_, k_, k1_(f(k_)};
        }

        auto end() const
        {
            return forward_iterator{f,sentinal()};
        }
    };

    auto operator()(key_type const & key) const
    {
        return map_proxy{f, hash(key)};
    }

    auto is_key(key_type const & key) const
    {
        return p(key);
    }

private:
    F f;
};
