#include <string_view>
#include <iostream>
#include <functional>

template <size_t N>
struct hash_value
{
    char buf[N];

    char & operator[](size_t index) { return buf[index]; }
    char operator[](size_t index) const { return buf[index]; }

    hash_value() = default;

    hash_value(std::string_view s)
    {
        for (size_t i = 0; i < N; ++i)
            buf[i] = i < s.size() ? s[i] : 0;
    }

    constexpr auto begin() const { return std::begin(buf); }
    constexpr auto end() const { return std::end(buf); }
    constexpr auto size() const { return N; }
    constexpr auto c_str() const { return buf; }
};

namespace std
{
    template <size_t N>
    struct hash<hash_value<N>>
    {
        size_t operator()(hash_value<N> const & x) const
        {
            size_t s = 0;
            for (size_t i = 0; i < N; ++i)
                s ^= x[i] + 0x9e3779b9 + (s << 6) + (s >> 2);
            return s;
        }
    };
}

template<size_t N1, size_t N2>
auto append(hash_value<N1> l, hash_value<N2> r)
{
    hash_value<N1+N2> lr;
    for (size_t i = 0; i < N1; ++i)
        lr[i] = l[i];
    for (size_t i = 0; i < N2; ++i)
        lr[N1+i] = r[i];
    return lr;
}

template <size_t N>
auto operator^(hash_value<N> l, hash_value<N> r)
{
    hash_value<N> x;
    for (size_t i = 0; i < N; ++i)
        x[i] = r[i] ^ l[i];
    return x;
}

int main()
{
    auto x = hash_value<32>("hello, world");
    std::cout << x.c_str() << std::endl;

    auto a = hash_value<32>("foo");
    auto b = hash_value<32>("bar");
    auto c = append(a,b);
    std::cout << c.c_str() << std::endl;

    for (size_t i = 0; i < c.size(); ++i)
    {
        if (c.c_str()[i] == NULL)
            std::cout << "_";
        std::cout << c.c_str()[i];
    }
    std::cout << "\n";
}
