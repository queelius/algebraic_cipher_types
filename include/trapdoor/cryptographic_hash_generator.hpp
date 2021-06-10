/**
 * A cryptographic hash generator G must provide overloads of the types
 *     G : () -> Hash,
 *     update : G & -> unsigned char * -> G::size_type -> G &,
 * and
 *     entropy : G const & -> Number.
 * 
 * Since G is a hash generator, it accepts zero or more byte sequences using
 * the update procedure and may be invoked as a function to generate the hash
 * of the byte sequences fed to it.
 * 
 * Conceptually, G models a function of type
 *     G : ByteSequence -> Hash
 * but we break it up into two separate stages for computational efficeincy.
 * Rather than giving it the entire ByteSequence at once, we allow the
 * programmer to feed it byte sequences, one after the other, until there are
 * no more left. In other words, it is an online algorithm.
 * 
 *     G : () -> Hash
 * where Hash models the concept of a hash value corresponding to the state of
 * the generator G.
 * 
 * The hash of the byte sequences x1,x2,...,xn is given by
 *     G.update(x1).update(x2)...(xn)().
 * The hash of the empty string, G(), is the identity element over |,
 *     G() | G.update(x1).update(x2)...(xn)() == G.update(x1).update(x2)...(xn)()
 * and is the zero element over &,
 *     G() & G.update(x1).update(x2)...(xn)() == G().
 *
 * A cryptographic hash generator G must provide overloads of the type
 *     G : (input: unsigned char *, n: G::size_type) -> G &
 * which is not a function but a procedure that mutates the state of G
 * and returns a non-const reference as output (to facilitate chaining).
 * 
 * input is a pointer to a byte (char) sequence.
 * 
 * n is the number of bytes from the sequence, starting at the address being
 * pointed to by input, to apply the md5 generator.
 * 
 * For an object to be cryptographically hashed by the generator G, there must
 * be a way to map the object to a sequence of bytes. If the mapping is
 * injective, T -> (unsigned char*, G::size_type), then the probability that
 * two non-identical objects of type T collide is given by
 * 
 *     2^-G::entropy()
 * 
 * where G::entropy() is the Shannnon entropy of G. Generally, G::entropy()
 * is only an estimate, and may change over time. The maximum entropy is given
 * by k where k is the bit length of the fixed-size hash values.
 * 
 * A second-preimage attack is the problem of for a given x, find an x' s.t.
 * h(x) = h(x'), where each x' has probability 2^-G::entropy(). A collision
 * attack is when we try to find any two x,x' s.t. h(x) = h(x'), which has
 * probability 2^(-G::entropy/2). Note that, with respect to this attack,
 * even a maximum entropy hash function generator
 * 
 * A random oracle is a theoretical device that maps bit sequences of arbitrary
 * length to bit sequences of infinite length such that, a priori, each bit
 * sequence in the domain has equal probability of being mapped to any element
 * in the codomain. Clearly, the entropy is thus infinite. If we concatenate
 * the output of the random oracle to k bits, we have an ideal cryptographic
 * hash function with entropy 2^k.
 * 
 * Note: If the mapping function is not injective, i.e., different objects of
 * type T map to the same byte sequence, then this introduces another way in
 * which objectives may collide and thus 2^(-G::entropy()) is the least
 * lower-bound for the probability of a collision.
 * 
 * If we are interested in the probability of collision over some set of types
 * {T1,...,Tm}, then the mapping function is also a function of the type.
 */

#include "cryptographic_hash.hpp"

// type-erasure over concrete types that model cryptographic hash generators.
// It assumes the output hash of the erased type has a function
//     hexadecimal : Hash -> string
// and thus cryptographic_hash_generator is a hash generator type that
// models
//     unsigned char* -> size_type -> cryptographic_hash
//
// 
class cryptographic_hash_generator
{
public:
    using size_type = size_t;

    auto & update(unsigned char * input, size_type n)
    {
        concept_->update(input,n);
        return *this;
    }

    cryptographic_hash operator()() const
    {
        return concept_->generate();
    }

    // The Shannon entropy of the hash generator.
    //
    // The maximum entropy of a cryptographic hash function of type
    //     {0,1}^* -> {0,1}^128
    // has 128 bits of entropy, so the md5 algorithm is reasonably close to
    // obtaining this ideal with an estimated 127.3 bits of entropy.
    //
    // Given a hash h(x), the expected number of trials needed to find an object
    // y s.t. hash(y) = hash(x) is given by
    //     2^(entropy()-1).
    auto entropy() const { return concept_->entropy(); }

    cryptographic_hash_generator(cryptographic_hash_generator const & copy) :
        concept_(copy.concept_->clone()) {}

    template <typename G>
    cryptographic_hash_generator(G const & g) :
        concept_(new model<G>{g}) {}

private:
    struct concept
    {
        virtual void update(unsigned char * input, size_type n) = 0;
        virtual cryptographic_hash generate() const = 0;
        virtual double entropy() const = 0;
        virtual concept * clone() const = 0;
    };

    std::unique_ptr<concept> concept_;

    template <typename G>
    struct model final : concept
    {
        G g;

        std::string generate() const override { return hexadecimal(g()); }
        concept * clone() const { return std::unique_ptr<concept>{new G(g)}; }
        double entropy() const override { return static_cast<double>(entropy(g)); }
        void update(unsigned char * input, size_type n) override { G.update(input,n); }
    };
}


template <typename G>
auto entropy(G const & g)
{
    return g.entropy();
}

template <typename G>
G & update(
    G & g,
    unsigned char const * bytes,
    typename G::size_type n)
{
    return g.update(bytes, n);
}

template <typename G>
G & update(
    G & g,
    char const * input,
    typename G::size_type n)
{
    return update(g, (unsigned char const *)input, n);
}

// I models an input iterator (single-pass forward iterator)
// G models a cryptographic hash function generator
template <typename I, typename G>
G & update(
    G & g,
    I begin,
    I end)
{
    for (auto x = begin; x != end; ++x)
        return update(g, value(*x), n);
    return g;
}
