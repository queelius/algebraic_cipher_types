/**
 * Boolean algebra
 *     B = ({0,1}^8k, xor, and, id, 0^8k, 1^8k)
 * which is isomorphic to the Boolean algebra
 *     A = (2^X, set-symmetric-diff, set-intersection, complement, {}, X),
 * where |X| = 8k.
 * 
 * The isomorphism F : A -> B is defined in the following way:
 *     1) Each element in X is mapped one-to-one to some bit string in
 *        {0,1}^(8k) in which only one of the bits is set to 1, e.g.,
 *        if |X| = 8, then F x may map to, say, 00000100.
 *     2) F set-symmetric-diff = xor.
 *     3) F set-intersection = and.
 *     4) F complement = id.
 *     5) F {} = 0^8k.
 *     6) F X = 1^8k.
 * 
 * Note that set-union of X and Y in Boolean algebra B is equivalent to
 *     (X xor Y) xor (X and Y).
 * 
 * The subset relation <= has a predicate
 *     <= : {0,1}^8k -> {0,1}^8k -> bool
 * which may be defined as
 *     X <= Y := X & Y == X
 * which also models the is-member relation when the first argument has only
 * one bit set.
 * 
 * Consider another Boolean algebra
 *     C = ({0,1}^8t, xor, and, id, 0^8t, 1^8t)
 * with the homorphism G : B -> C defined as:
 *     1) Suppose x is an element in B. G := hash, where hash : B^* -> B^8t
 *        is a crypographic hash function that is both non-injective and
 *        non-surjective. It is also pre-image resistant, i.e., given a y,
 *        finding an x such that y = hash(x) is "hard."
 *     2) G xor = xor
 *     3) G and = and
 *     4) G id = id
 *     5) G 0^8k = 0^8t
 *     6) G 1^8k = 1^8t.
 * 
 * G : B -> C is a homomorphism since
 *     G (x xor y) = (G x) (G xor) (G y).
 * Proof:
 *     sketch here
 * 
 * However, the Boolean algebra C has no subset relation that can be practically
 * be defined. Moreover, the homomorphism G yields a Boolean algebra C that only
 * *approximates* B. In fact, the approximation is fist-order random approximate
 * set, which is the random approximate set with the highest entropy.
 * 
 * The only relation it has a predicate for is the equality relation,
 *     == : {0,1}^8t -> {0,1}^8t -> bool,
 * which has an error given by ???.
 * 
 * 
 * The kernel of A with respect to C is given by the subsets in X that map
 * to the zero vector. Since only the empty set should map to the zero vector,
 * we say that if the kernel is *expected* to be greater than 1, then C is a
 * random approximate set. In this case, a first-order random approximate set.
 * 
    template <typename X>
    //     auto ker(approx_value<X> type) :=
    //         { x in X | identity<approx_type>::value == approx_type<X>(x) }.

* 
 */

template <typename X, size_t N>
struct trapdoor_symmetric_difference_group
{
    using value_type = X;

    trapdoor_symmetric_difference_group() :
        value_hash(0),
        key_hash(0)
    {
        // makes the empty set
    }

    trapdoor_set(trapdoor_set const & copy) :
        value_hash(copy.value_hash),
        key_hash(copy.key_hash) {}

    array<char,N> value_hash;
    array<char,4> key_hash;
};

/**
 * This is the only function implicitly defined for the type.
 * Other functions of the type must use a cipher map.
 */
template <typename X, size_t N>
approximate_bool operator==(
    trapdoor_symmetric_difference_group<X,N> const & lhs,
    trapdoor_symmetric_difference_group<X,N> const & rhs)
{
    return approximate_bool{lhs.value_hash == rhs.value_hash, .5};
}

template <typename X, size_t N>
auto make_empty_trapdoor_symmetric_difference_group()
{
    return trapdoor_symmetric_difference_group<X,N>();
}

/**
 * The disjoint union operation is a partial function that is only defined
 * when the argument sets are disjoint (it is a dependent type). If they are
 * not disjoint, the operation has undefined behavior.
 */
template <typename X, size_t N>
auto operator+(
    trapdoor_symmetric_difference_group<X,N> const & x,
    trapdoor_symmetric_difference_group<X,N> const & y)
{
    if (key_hash(x) != key_hash(y))
        throw invalid_argument("secret key mismatch");

    // since xor (^) is assocative and commutative,
    //     + : trapdoor_symmetric_difference_group<X> -> trapdoor_symmetric_difference_group<X> -> trapdoor_symmetric_difference_group<X>
    // is also assocative and commutative.
    return trapdoor_symmetric_difference_group<X>(
        hash(x) ^ hash(y),
        key_hash(x));
}

template <typename X>
auto operator+(trapdoor_symmetric_difference_group<X> const & xs, trapdoor<X> const & x)
{
    if (key_hash(x) != key_hash(y))
        throw invalid_argument("secret key mismatch");

    return trapdoor_symmetric_difference_group<X>(
        hash(xs) ^ hash(x),
        key_hash(xs));
}

/**
 * We see that the empty set is the 0's string.
 * Since a hash can map to all 0's, and xoring values may also, there is
 * some probability that a non-empty set will map to all zeros.
 */
template <typename X>
approximate_bool empty(trapdoor_symmetric_difference_group<X> const & xs)
{
    // additive identity is the zero bit string.
    return approximate_bool{xs.hash_value == 0,.5};
}

