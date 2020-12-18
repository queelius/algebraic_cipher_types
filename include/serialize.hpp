#include <utility>

template <typename X, typename Y>
struct unpack
{
}

template <typename X, typename Y>
struct pack
{
}

template <>
struct unpack<uint64_t, pair<uint32_t,uint32_t>>
{
    static auto operator()(uint64_t x)
    {
        return { x >> 32, x & 0x00000000ffffffff };
    }
}

template <>
struct pack<pair<uint32_t,uint32_t>,uint64_t>
{
    static auto operator()(pair<uint32_t,uint32_t> x)
    {
        uint64_t r = x.first;
        r <<= 32;
        r |= x.second;
        return r;
    }
}


template <>
struct unpack<uint32_t, pair<uint16_t,uint16_t>>
{
    static auto operator()(uint32_t x)
    {
        return { x >> 16, x & 0x0000ffff };
    }
}

template <>
struct unpack<uint16_t, pair<uint8_t,uint8_t>>
{
    static auto operator()(uint16_t x)
    {
        return { x >> 8, x & 0x00ff };
    }
}

