#pragma once

/**
 * Type info tells us what the bits of
 * a value represents, e.g., in C++ a value
 * of type bool has a false value
 * represented by a word of all zeros and any
 * bit pattern represents true.
 * 
 * A cipher tag provides obfuscated type
 * information (note that some cipher value
 * types may have plaintext type information,
 * e.g., a cipher<bool> as opposed to
 * cipher<cipher<bool>>).
 * 
 * A cipher tag is simply a form of encryption
 * of a plaintext tag. Normally, we just use
 * encryption, but we may also use a one-way
 * has to construct a trapdoor for a tag.
 * 
 * In either case, whether invertible or
 * a trapdoor, if the secret is known, then we
 * can at least verify that a cipher tag is a
 * specified type.
 * 
 * If the tag is encrypted, its length reveals
 * something about the type, e.g., whether it
 * is an algebraic composition or an especially
 * short tag. In this case, padding techniques
 * may be used such that every cipher tag is
 * some fixed size.
 * 
 * If, instead, we use a trapdoor, we may simply
 * map each tag to some one-way hash seeded by
 * the secret key. In this case, the cipher tag
 * is an approximate cipher tag since there is a
 * non-zero probability that different tags map
 * to the same hash.
 */


#include <string>
#include <cmath>
#include <utility>
using std::pair;
using std::string;
using std::size_t;
using std::make_pair;
using std::strtoull;
using std::to_string;

namespace alex::cipher
{
    struct trapdoor_tag
    {
        using value_type = size_t;

        value_type value;

        /**
         * Suppose we have two approximate trapdoor tags a and b.
         * 
         * The false negative rate on tag equality is zero
         * (positive approximate set). The false positive rate on tag
         * equality is
         *           
         *     fpr = 2^(-k)
         * 
         * where k is the bit length of the trapdoor tags.
         * 
         * By the assumption of Kerckhoff's principle, the adversary
         * knows the bit length of the trapdoors of primitive tags,
         * i.e., BL(trapdoor(T)) = k for all T in the set of primitive
         * types, like int and bool.
         * 
         * Thus, if trapdoor tags are being algebraically composed, e.g.,
         * 
         *     W := trapdoor(T) + trapdoor(U),
         * 
         * then the type is not a complete trapdoor since the adversary
         * can determine that is a composition of some sort by, at the very
         * least, observing that BL(W) >= 2k.
         * 
         * If the nature of the composition must be kept secret, then
         * a cipher of the composition may be used, e.g.,
         * 
         *     trapdoor(T+U).
         * 
         * Of course, this makes it more difficult to compose cipher
         * functions with type checking (type information may always
         * be ignored, of course, but that introduces another element
         * of approximation error and program incorrectness).
         */
        auto operator==(trapdoor_tag const & rhs) const
        {
            return approximate_pos_neg<2,bool>{
                std::pow(2.,sizeof(value_type) * 8.), // fpr
                0.,                                   // fnr
                rhs.value == value};
        }

        auto operator!=(trapdoor_tag const & rhs) const
        {
            return !(*this == rhs);
        }
    };        

    template <typename T>
    struct serialize {};

    template <typename T>
    struct deserialize {};

    template <>
    struct deserialize<trapdoor_tag>
    {
        template <typename I>
        pair<optional<trapdoor_tag>,I> operator()(I begin, I end) const
        {
            if (begin == end)
                return make_pair(nullopt,begin);

            auto tag = (trapdoor_tag::value_type)strtoull(*begin++,nullptr,10);
            return make_pair(move(tag),begin);
        }
    };

    template <>
    struct serialize<trapdoor_tag>
    {
        template <typename O>
        pair<bool,O> operator()(O out) const
        {
            
            auto tag = (trapdoor_tag::value_type)strtoull(*begin++,nullptr,10);
            return make_pair(move(tag),begin);
        }
    };


}