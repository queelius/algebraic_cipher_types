

/**
 * cipher_cardinal<N> is a cipher type
 *     ({0,1,...,N-1},{<,==})
 * where the computational basis {<,==} may be used to derive {<,<=,==,!=,>,>=}.
 *
 * The less-than and equality predicates
 *     < : cipher_cardinal<N> -> cipher_cardinal<N> -> cipher_bool
 * and
 *     == : cipher_cardinal<N> -> cipher_cardinal<N> -> cipher_bool
 * may be modeled as cipher binary relations over cipher_cardinal<N>.
 */

template <size_t N>
class cipher_cardinal
{
    // A trapdoor of a natural number is a one-way transformation.
    // A natural number x that is a cipher is a two-way transformation
    // that has a pair of functions
    //
    size_t cipher;

    // the key hash is a hash of the secret key,
    // which faciliates a form of dynamic type checking.
    size_t key_hash;

};

template <size_t N, size_t K, typename H>
struct cipher_cardinal_coder
{
    template <typename S>
    cipher_cardinal<N> operator()(size_t x, S s) const
    {
        for (size_t i = 0; i < K; ++i)
            x = h(x);        
        return x ^ h(s);
    }

    H h;
};

struct cipher_cardinal_decoder
{
    template <typename S>
    cipher_cardinal<N> operator()(size_t x, S s) const
    {
        for (size_t i = 0; i < K; ++i)
            x = h(x);        
        return x ^ h(s);
    }

    H h;
};
   


template <>
struct cipher_less_than<cipher_cardinal>
{
    cipher_bool less_than(cipher_cardinal lhs, cipher_cardinal rhs)
    {

    }
};
