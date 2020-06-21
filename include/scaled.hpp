/**
 * Given that type T is a ring, denoted by
 *     G = (T,+,*,-,T(0))
 * we define a related ring, denoted by
 *     H = (scaled<T,N,D>,+,*,-,scaled<T,N,D>(T(0))).
 * 
 * We define the group homomorphisms G -> H and H -> G
 * where
 *     G := scaled<T,N,D>(x * N/D)
 * and
 *     H := T(x * D/N)
 * that allow converting between these two groups.
 * Thus, * : T -> T and / : T -> T must be
 * implemented with the usual meaning. Note that
 * H is a *partial* function, since some scaled
 * values will necessarily be outside the range.
 * 
 * scaled<T,N,D> has the range
 * [-a*M,a*M], where M := numeric_limits<T>::max()
 * and a = D/N. If -1 < N/D < 1, then the range
 * is increased.
 * 
 * Most operations on T can be lifted to operations
 * on scaled<T> trivially.
 * 
 * TODO: Is this a category? I think so. Investigate.
 * 
 * Even if the result must eventually be converted
 * back into a value of type T, it may be the case
 * that only *intermediate* values in a computation
 * would overflow T, but the final result may be
 * converted to type T without overflowing.
 * 
 * We provide a predicate,
 * 
 *     template <typename S>
 *     overflow_to : scaled<T> -> bool,
 * 
 * to determine if scaled<T> when converted to a
 * value of type S causes an overflow.
 * 
 * 
 * 
 * 
 * Example uses:
 * 
 * Suppose we wish to compute the likelihood of some
 * sample under some probability model with parametric
 * pdf
 *     p : X -> T
 * where T models some real number type capable
 * of representing the values in the set [0,1].
 * 
 * We store most of our numberical data as values
 * of type double, but suspect that the likelihood,
 * as a product of p, will be too small to represent
 * as a double (underflow).
 * 
 * We decide to parameterize p as
 *     p<lg<T>>
 * so that, underneath the hood, multiplications
 * are converted into additions and the numbers
 * are internally represented as their respective
 * exponents.
 * 
 * The result of the likelihood computation is
 * a value of type lg<T>. We can perform many
 * operations on this result, including comparisons,
 * with the exception of addition and subtraction,
 * which is generally not needed in such cases
 * anyway.
 * 
 */

#include <cmath>
using std::log;
using std::exp;
using std::numeric_limits;

template <typename T, int N, int D>
struct scaled
{
    T k;

    static constexpr auto scale() const { return T(N) / T(D); }

    // by default, construct a value equal to 1.
    scaled(T x = 0) : k(x * scale()) {}

    // operator to convert to type T.
    operator T() const { return x / scale(); }
};

template <typename T, int N, int D>
struct numeric_limits<scaled<T,N,D>>
{
    // If T has max() of M, then scaled<T,N,D> has max of M * D / N.
    // Thus, if D > N, then max<scaled<T,N,D>> is greater than max<T>.
    // There are other reasons to scale, of course, e.g., to avoid
    // working with really small ill-conditioned numbers, in which case
    // D < N to scale up the internal representation of the number in
    // scaled<T,N,D>.
    static constexpr auto max() { return scaled<T,N,D>{number_limits<T>::max()}; }
    static constexpr auto is_signed() { return true; }
    static constexpr auto has_infinity() { return numeric_limits<T>::has_infinity(); }
    static constexpr auto infinity() { return scaled<T,N,D>{numeric_limits<T>::infinity()}; }
};

template <typename T, int N, int D>
auto log(scaled<T,N,D> const & x)
{
    static constexpr alpha = log(T(D)) - log(T(N));
    return scaled<T,N,D>{log(x.k) + alpha};
}

template <typename T, int N, int D>
auto exp(scaled<T,N,D> const & x)
{
    static constexpr alpha = T(D) / T(N);
    return scaled<T,N,D>{exp(alpha * x.k)};
}

template <typename T, int N, int D>
auto overflow_to(scaled<T,N,D> const & x) { return numeric_limits<T>::max() <= x.k * x.scale(); }

template <typename T, int N, int D>
auto inv(scaled<T,N,D> const & x) { return scaled<T,N,D>{-x.k}; }

template <typename T, int N, int D>
auto operator*(scaled<T,N,D> const & x, scaled<T,N,D> const & y) { return scaled<T,N,D>{x.k * y.k * x.scale()}; }

template <typename T, int N, int D>
auto operator/(scaled<T,N,D> const & x, scaled<T,N,D> const & y) { return scaled<T,N,D>{x.k / y.k * x.scale()}; }

template <typename T, int N, int D>
auto operator+(scaled<T,N,D> const & x, scaled<T,N,D> const & y) { return scaled<T,N,D>{x.k + y.k}; }

template <typename T, int N, int D>
auto operator-(scaled<T,N,D> const & x, scaled<T,N,D> const & y) { return scaled<T,N,D>{x.k - y.k}; }

template <typename T, int N, int D>
auto operator<(scaled<T,N,D> const & x, scaled<T,N,D> const & y) { return x.k < y.k; }

template <typename T, int N, int D>
auto operator<=(scaled<T,N,D> const & x, scaled<T,N,D> const & y) { return x.k <= y.k; }

template <typename T, int N, int D>
auto operator==(scaled<T,N,D> const & x, scaled<T,N,D> const & y) { return x.k == y.k; }

template <typename T, int N, int D>
auto operator!=(scaled<T,N,D> const & x, scaled<T,N,D> const & y) { return x.k != y.k; }

template <typename T, int N, int D>
auto operator>(scaled<T,N,D> const & x, scaled<T,N,D> const & y) { return x.k > y.k; }

template <typename T, int N, int D>
auto operator>=(scaled<T,N,D> const & x, scaled<T,N,D> const & y) { return x.k >= y.k; }
