#pragma once
#include <cstdio>
#include <array>
#include "md5_hash.hpp"

/**
 * md5 is a functor that models the concept of a cryptographic hash function
 * using the popular MD5 algorithm.
 * 
 * md5 is able to hash any Hashable object types, thus md5 is conceputally
 * of type
 *     Hashable -> md5_hash
 * where md5_hash has around 2^md5::entropy() possible values and an object
 * type T models Hashable if there exists a function
 *     value : T -> C
 * where C models a one-pass iterable collection over elements of type
 *     pair<unsigned char*,size_t>.
 * 
 * Everything is based on the more primitive
 *     md5 : (unsigned char*, size_t) -> md5_hash,
 * and thus md5 is also able to hash contiguous byte sequences with a specified
 * size.
 * 
 * Note that md5_hash also models Hashable, so if we let . represent the
 * composition operator, then
 *     md5 . md5 : Hashable -> md5_hash
 * is defined. Since the md5 hash of md5_hash is not the identity, we see that
 * md5 . md5 is not identical to md5 and so technically is no longer of type
 *     Hashable -> md5_hash
 * but of some other type
 *     Hashable -> Hash.
 * 
 * md5 . md5 is a cryptographic hash function with a lower entropy than md5.
 * Why? Mapping a Hashable to md5_hash will have
 *     2^md5::entropy()
 * possible values. Since md5 is not injective (nor surjective), when we hash
 * these possible md5_hash values, some of them collide, resulting in even
 * fewer possible values.
 * 
 * If we denote md5^k as k compositions of md5, then
 *     limit k->infinity md5^k
 * has some minimum entropy that is (probably) significantly less than
 * md5::entropy(). It is unlikely to have zero entropy, since at some point
 * the md5_hash values will likely cycle, but at that point the composition
 * may have a small enough entropy that it is broken even against preimage
 * attacks.
 */
class md5
{
public:
    friend class md5_hash;

    using size_type = uint32_t;

    /**
     * The Shannon entropy of the md5 hash function.
     *
     * The md5 function is neither surjective nor completely equally distributed
     * over its image, so the entropy is not 128 bits, but estimated to be
     * around 127.3 bits.
     */
    static auto entropy() const { return 127.3; }

    md5() { reset(); }

    /**
     * md5_hash permits online construction of hash values. We finalize a hash
     * construction by applying it to whatever was previously fed to it
     * with updates,
     * 
     * For instance, if
     *     char x[] = {b1,b2,b3},
     * and
     *     h := md5(x,3)()
     * then
     *     md5(&b1,1).(&b2,1).(&b3,1)() ==
     *     md5(&b1,1).(&b2,2)() ==
     *     md5(&b1,2).(&b2,1)() ==
     *     md5(&b1,3)() == h
     * is true.
     */
    md5_hash operator()() const
    {
        unsigned char padding[64] = {
            0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        unsigned char bits[8];
        encode(bits, count, 8);
        size_type index = count[0] / 8 % 64;
        update(padding, (index < 56) ? (56 - index) : (120 - index));
        update(bits, 8);

        std::array<char,16> value;
        encode(value, state, 16);
        reset();
        return md5_hash(std::move(value));
    }

    // T models Hashable
    template <typename T>
    auto operator()(T const & x)
    {
        reset();
        for (auto v : value(x))
            operator()(v.first,v.second);
        return operator()();
    }

    // a special overload for string_view
    //     md5 : string_view -> md5_hash
    auto operator()(std::string_view x) { return operator()(x.data(),x.size())(); }

    // we provide a special overload for contiguous chars
    md5 & operator()(char const * x, size_type n) { return operator()(static_cast<unsigned char*>(x),n); }

    md5 & operator()(unsigned char const * x, size_type n)
    {
        // compute number of bytes mod 64
        size_type index = count[0] / 8 % blocksize;

        // update number of bits
        if ((count[0] += (n << 3)) < (n << 3))
            count[1]++;
        count[1] += (n >> 29);

        // number of bytes we need to fill in buffer
        size_type firstpart = 64 - index;

        size_type i;

        // transform as many times as possible.
        if (n >= firstpart)
        {
            // fill buffer first, transform
            memcpy(&buffer[index], input, firstpart);
            transform(buffer);

            // transform chunks of blocksize (64 bytes)
            for (i = firstpart; i + blocksize <= n; i += blocksize)
                transform(&x[i]);

            index = 0;
        }
        else
            i = 0;

        // buffer remaining input
        memcpy(&buffer[index], &x[i], n - i);

        return *this;
    }

private:
    static const int S11 = 7;
    static const int S12 = 12;
    static const int S13 = 17;
    static const int S14 = 22;
    static const int S21 = 5;
    static const int S22 = 9;
    static const int S23 = 14;
    static const int S24 = 20;
    static const int S31 = 4;
    static const int S32 = 11;
    static const int S33 = 16;
    static const int S34 = 23;
    static const int S41 = 6;
    static const int S42 = 10;
    static const int S43 = 15;
    static const int S44 = 21;
    static const int blocksize = 64;

    // decodes input (uint8_t) into output (uint32_t).
    // assumes n is a multiple of 4.
    static void decode(uint32_t output[], const uint8_t input[], size_type n)
    {
        for (size_t i = 0, j = 0; j < n; i++, j += 4)
            output[i] = ((uint32_t)input[j]) | (((uint32_t)input[j + 1]) << 8) |
                        (((uint32_t)input[j + 2]) << 16) | (((uint32_t)input[j + 3]) << 24);
    }

    void reset()
    {
        // load magic initialization constants.
        state[0] = 0x67452301;
        state[1] = 0xefcdab89;
        state[2] = 0x98badcfe;
        state[3] = 0x10325476;

        // zero out sensitive information.
        memset(buffer, 0, sizeof buffer);
        memset(count, 0, sizeof count);
    }

    void transform(const uint8_t block[blocksize])
    {
        uint32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];
        decode(x, block, blocksize);

        /* Round 1 */
        FF(a, b, c, d, x[0], S11, 0xd76aa478);  /* 1 */
        FF(d, a, b, c, x[1], S12, 0xe8c7b756);  /* 2 */
        FF(c, d, a, b, x[2], S13, 0x242070db);  /* 3 */
        FF(b, c, d, a, x[3], S14, 0xc1bdceee);  /* 4 */
        FF(a, b, c, d, x[4], S11, 0xf57c0faf);  /* 5 */
        FF(d, a, b, c, x[5], S12, 0x4787c62a);  /* 6 */
        FF(c, d, a, b, x[6], S13, 0xa8304613);  /* 7 */
        FF(b, c, d, a, x[7], S14, 0xfd469501);  /* 8 */
        FF(a, b, c, d, x[8], S11, 0x698098d8);  /* 9 */
        FF(d, a, b, c, x[9], S12, 0x8b44f7af);  /* 10 */
        FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
        FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
        FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
        FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
        FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
        FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

        /* Round 2 */
        GG(a, b, c, d, x[1], S21, 0xf61e2562);  /* 17 */
        GG(d, a, b, c, x[6], S22, 0xc040b340);  /* 18 */
        GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
        GG(b, c, d, a, x[0], S24, 0xe9b6c7aa);  /* 20 */
        GG(a, b, c, d, x[5], S21, 0xd62f105d);  /* 21 */
        GG(d, a, b, c, x[10], S22, 0x2441453);  /* 22 */
        GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
        GG(b, c, d, a, x[4], S24, 0xe7d3fbc8);  /* 24 */
        GG(a, b, c, d, x[9], S21, 0x21e1cde6);  /* 25 */
        GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
        GG(c, d, a, b, x[3], S23, 0xf4d50d87);  /* 27 */
        GG(b, c, d, a, x[8], S24, 0x455a14ed);  /* 28 */
        GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
        GG(d, a, b, c, x[2], S22, 0xfcefa3f8);  /* 30 */
        GG(c, d, a, b, x[7], S23, 0x676f02d9);  /* 31 */
        GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

        /* Round 3 */
        HH(a, b, c, d, x[5], S31, 0xfffa3942);  /* 33 */
        HH(d, a, b, c, x[8], S32, 0x8771f681);  /* 34 */
        HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
        HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
        HH(a, b, c, d, x[1], S31, 0xa4beea44);  /* 37 */
        HH(d, a, b, c, x[4], S32, 0x4bdecfa9);  /* 38 */
        HH(c, d, a, b, x[7], S33, 0xf6bb4b60);  /* 39 */
        HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
        HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
        HH(d, a, b, c, x[0], S32, 0xeaa127fa);  /* 42 */
        HH(c, d, a, b, x[3], S33, 0xd4ef3085);  /* 43 */
        HH(b, c, d, a, x[6], S34, 0x4881d05);   /* 44 */
        HH(a, b, c, d, x[9], S31, 0xd9d4d039);  /* 45 */
        HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
        HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
        HH(b, c, d, a, x[2], S34, 0xc4ac5665);  /* 48 */

        /* Round 4 */
        II(a, b, c, d, x[0], S41, 0xf4292244);  /* 49 */
        II(d, a, b, c, x[7], S42, 0x432aff97);  /* 50 */
        II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
        II(b, c, d, a, x[5], S44, 0xfc93a039);  /* 52 */
        II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
        II(d, a, b, c, x[3], S42, 0x8f0ccc92);  /* 54 */
        II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
        II(b, c, d, a, x[1], S44, 0x85845dd1);  /* 56 */
        II(a, b, c, d, x[8], S41, 0x6fa87e4f);  /* 57 */
        II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
        II(c, d, a, b, x[6], S43, 0xa3014314);  /* 59 */
        II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
        II(a, b, c, d, x[4], S41, 0xf7537e82);  /* 61 */
        II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
        II(c, d, a, b, x[2], S43, 0x2ad7d2bb);  /* 63 */
        II(b, c, d, a, x[9], S44, 0xeb86d391);  /* 64 */

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;

        // Zeroize sensitive information.
        memset(x, 0, sizeof x);

    }

    uint8_t buffer[blocksize]; // bytes that didn't fit in last 64 byte chunk
    uint32_t count[2];         // 64-bit counter for number of bits (lo, hi)
    uint32_t state[4];         // digest so far

    // low level logic operations
    static uint32_t F(uint32_t x, uint32_t y, uint32_t z) { return x & y | ~x & z; }
    static uint32_t G(uint32_t x, uint32_t y, uint32_t z) { return x & z | y & ~z; }
    static uint32_t H(uint32_t x, uint32_t y, uint32_t z) { return x ^ y ^ z; }
    static uint32_t I(uint32_t x, uint32_t y, uint32_t z) { return y ^ (x | ~z); }
    static uint32_t rotate_left(uint32_t x, int n) { return (x << n) | (x >> (32 - n)); }
    static void FF(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) { a = rotate_left(a + F(b, c, d) + x + ac, s) + b; }
    static void GG(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) { a = rotate_left(a + G(b, c, d) + x + ac, s) + b; }
    static void HH(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) { a = rotate_left(a + H(b, c, d) + x + ac, s) + b; }
    static void II(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) { a = rotate_left(a + I(b, c, d) + x + ac, s) + b; }
};

