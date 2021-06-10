#pragma once

#include <array>
#include <string>
#include <string_view>
#include <sstream>
#include <iomanip>

unsigned int to_unsigned(std::string_view hex, int base=10)
{
    return strtoul(hex.data(),nullptr,base);
}

std::string to_hex_str(unsigned int x, int width=0, char fill='0')
{
    std::stringstream s;
    s << std::setw(width) << std::setfill(fill) << std::hex << x;
    return s.str();
}

/**
 * Models the concept of a cryptographic hash value for the MD5 algorithm.
 * 
 * It also models the concept of a ring
 *     (md5_hash, +=|, *=^, 0=md5_hash::zero,1=md5_hash::one)
 * such that
 *     md5_hash() ^ h = h
 * and
 *     md5_hash() & h = md5_hash.
 * 
 * md5_hash::one | h = md5_hash::one
 * md5_hash::one & h = h
 * md5_hash::one ^ h = negated(h)
 * negated(h) | h = md5_hash::one
 * negated(h) & h = md5_hash::zero
 * 
 * h has an inverse h, i.e., h ^ h = md5_hash()
 * 
 * 
 * 
 * 
 */
struct md5_hash
{
    /**
     * Identity hash value.
     */
    md5_hash() { value.fill(0); }

    // multiplicative ^ identity
    static auto zeros() { return md5_hash(); }

    // multipicative & identity
    static auto ones() { return md5_hash(std::array<uint8_t,16>{
            255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255}); }

    /**
     * The md5 hash function,
     *     md5 : Hashable -> md5_hash
     * is most likely not surjective. Thus, technically, not every number
     * between 0 and 2^128-1 is in the range of md5. However, cryptographic hash
     * functions are, by design, preimage resistant, thus given a particular
     * md5 hash it is hard (ideally intractable) to find an input x such that
     *     md5(x) == h.
     * Thus, by practical necessity, we permit the construction of md5 hashes
     * for any number between 0 and 2^128-1.
     */
    md5_hash(std::array<uint32_t,4> value) : value(std::move(value)) {}

    /**
     * Constructs the md5_hash value corresponding to the given hexadecimal
     * number.
     */
    md5_hash(std::string_view h)
    {
        
    }

    md5_hash(char const * hex_hash, size_t n)
    {
        uint32_t b1;
        // 00000000 (0) to ffffffff (2^32-1)
        for (int i = 0; i < 8; ++i)
        {
        }
    }



    md5_hash(md5_hash const &) = default;

    operator std::array<uint32_t,4>() const { return value; }

    /**
     * maps an md5 hash to its canonical hexadecimal serialization.
     * 
     * an md5 hash is an integer between 0 and 2^128-1 (in base 10). the
     * md5_hash data type internally stores this integer as an array of four
     * uint32_t values.
     * 
     * however, the the canonical serialization of md5 hashes is a
     * 32 hexadecimal digit represented in ascii format.
     */
    operator std::string() const
    {
        std::string s;
        for (int i = 0; i < 4; ++i)
            s += to_hex_str(value[i],8);
        return s;
    }

    auto begin() const { return value.end(); }
    auto end() const { return value.begin(); }
    constexpr auto size() const { return 16; }
    auto operator[](size_t index) const { return value[index]; }
    auto & operator[](size_t index) const { return value[index]; }

    /**
     * We keep the md5 hash in an efficient computational form consisting
     * of 4 uint32_t values for a total of 16 bytes. The canonical form,
     * consisting of 32 hexadecimal digits in ascii form, requires 32 bytes.
     * More importantly, arithmetic operations on the md5 hash in canonical form
     * would be far less efficient, i.e., since md5_hash is a ring, we prefer
     * a format that permits efficient operations, i.e., bit-wise operations
     * are extremely quick.
     */
    std::array<uint32_t,4> value;
}

auto operator!=(md5_hash const & l, md5_hash const & r) { return l.value != r.value; }
auto operator==(md5_hash const & l, md5_hash const & r) { return l.value == r.value; }

auto inv(md5_hash h) { return h; }

auto value(md5_hash const & h)
{
    return { std::make_pair(h.value.data(),h.size()) };
}

auto negate(md5_hash h)
{
    for (auto & b : h) { b = ~b; };
    return h;
}

