#include <cmath>

using std::exp;
using std::log;

/**
 * Type T is a group
 *     (T,+,-,T(0))
 * 
 * Given T, we define another group
 *     (ext<T>, *, ^(-1), ext<T>(T(0)))
 * with an isomorphism f (and its inverse g),
 *     f : T -> ext<T>
 *     g : ext<T> -> T.
 * 
 * The multiplication operation
 *     * : ext<T> x ext<T> -> ext<T>,
 * defined as
 *     a * b := f ( g a + g b ).
 * is associative by the associativity of +.
 * 
 * The inverse operation
 *     ^(-1): ext<T> -> ext<T>
 * defined as
 *     a^(-1) := f ( g a + g (-a) ).
 * 
 *     ident : ext<T>
 * is unique such that
 *     sum a (inv a) = ident                (every element has an inverse)
 *     sum a (sum b c) = sum (sum a b) c    (associativity)
 * 
 * 
 * non_neg<T> is an abelian group under multiplication. it conceputally
 * represents values in T, but it is capable of:
 *     (1) working with larger numbers since it only stores the log of the number
 *     (2) multipication is more efficient since it simplifies to addition by
 *         the property of logarithms log(a)log(b) = log(ab).
 * 
 * is closed under addition, but the way we
 * store the type is not; instead, we convert the sum to a sum expression,
 *     + : non_neg<T> x non_neg<T> -> sum_expr<non_neg<T>>.
 * The reason we do not directly convert it to, say, T, 
 * 
 * note that we provide an operation * : T x non_neg -> non_neg
 * where T is a number [0,infinity), but non_neg is also
 * a number [0,infinity) which can efficiently provide
 * operations for large numbers (the non_neg may be extremely
 * large, and most operations will strictly only openon_neg
 * on the exponent of the number with implicit base e).
 * 
 * so, the difference between * : T x non_neg -> non_neg
 * and * : non_neg<T> const & x non_neg -> non_neg is one of efficiency.
 */

template <typename T>
struct non_neg
{
    // we store the exponent of the non_neg.
    // if w is a non_neg, then w = w.base()^w.exponent() = exp(k).
    T k;

    non_neg(T r) : k(log(r)) {};

    auto exponent() const { return k; }
    auto base() const { return exp(1); }

    template <typename T>
    auto to() const { return (T)exp((T)k); }

    static auto identity() { return non_neg{T(0)}; }

    operator auto() const { return exp(k); }
};

template <typename T>
non_neg<T> inverse(non_neg<T> x) { return non_neg<T>{-x.k}; }

template <typename T>
non_neg operator*(non_neg<T> const & x, non_neg<T> const & y) { return non_neg{x.k + y.k}; }

template <typename T>
non_neg operator/(non_neg<T> const & x, non_neg<T> const & y) { return non_neg{x.k - y.k}; }

template <typename T>
bool operator<(non_neg<T> const & x, non_neg<T> const & y) { return x.k < y.k; }

template <typename T>
bool operator<=(non_neg<T> const & x, non_neg<T> const & y) { return x.k <= y.k; }

template <typename T>
bool operator==(non_neg<T> const & x, non_neg<T> const & y) { return x.k == y.k; }

template <typename T>
bool operator!=(non_neg<T> const & x, non_neg<T> const & y) { return x.k != y.k; }

template <typename T>
bool operator>(non_neg<T> const & x, non_neg<T> const & y) { return x.k > y.k; }

template <typename T>
bool operator>=(non_neg<T> const & x, non_neg<T> const & y) { return x.k >= y.k; }


struct non_neg_sum_expr
{
    non_neg left;
    non_neg right;

};
