#pragma once
#include <string_view>

// H models a cryptographic hash
template <typename H>
class pseudo_random_oracle
{
    struct truncated_hash
    {
        std::string value;
    }

    struct lazy_hash
    {
        H const & h;
        std::string_view x;

        char operator()(size_t index) const
        {
            auto hash = static_cast<std::string>(h(x.c_str(),x.size()));
            while (true)
            {
                if (index < size(h))
                    break;
                index -= size(h);
                hash = static_cast<std::string>(h(hash));
            }
            return hash[index];
        }

        auto truncate(size_t n) const
        {
            auto hash = h(x.c_str(),x.size());
            for (size_t i = 0; i < n; i += size(h))
            {
                hash = h(hash);
            }
        }
    }

    auto operator()(std::string_view x) const
    {
        return lazy_hash{h,x};
    }

    H h;
}

