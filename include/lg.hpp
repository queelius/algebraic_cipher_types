/**
 * Given that type T is a group, denoted by
 *     G = (T,+,-,T(0))
 * we define a related abelian group, denoted by
 *     H = (lg<T>, *, ^(-1), lg<T>(T(0))).
 * 
 * We define the group homomorphisms G -> H and H -> G
 * where
 *     G := lg<T>(log(x))
 * and
 *     H := T(exp(x))
 * that allow converting between these two groups.
 * Thus, log : T -> T and exp : T -> T must be
 * implemented with the usual meaning.
 * 
 * lg<T> is a non-negative number type that models
 * T, but with the the following features:
 * 
 *     (1) Efficiently implements multiplication (reduces to addition by
 *         the property of logarithms).
 * 
 *     (2) It has a much larger range [0,b] where
 *             b := exp(numeric_limit<T>::max()),
 *         but is only closed under multiplication.
 * 
 *         Even if the result must eventually be
 *         converted back into a value of type T,
 *         it may be the case that only *intermediate*
 *         values in a computation would overflow T,
 *         but the final result may be converted to
 *         type T without overflowing. We provide
 *         a will_overflow : lg<T> -> bool predicate
 *         to determine this before the conversion.         
 * 
 * is closed under addition, but the way we
 * store the type is not; instead, we convert the sum to a sum expression,
 *     + : lg<T> -> lg<T> -> sum_expr<lg<T>>.
 * The reason we do not directly convert it to, say, T, 
 * 
 * Note: an interesting underlying type T is one that accumulates
 * very little rounding error on addition, e.g., a type T that
 * implements + : T -> T -> T using the Kahan summation algorithm
 * such that after products of the form
 *     x1 * x2 * ... * xn,
 * which is transformed to
 *     log x1 + log x2 + ... + log xn,
 * has very little rounding error.
 */

#pragma once

#include <cmath>
#include "safe.hpp"

using std::exp;
using std::log;
using std::sqrt;
using std::numeric_limits;

template <typename T>
struct lg
{
    T k;

    // log : lg<T> -> lg<T>.
    auto log() const { return lg{k}; };

    lg(lg const &) = default;

    // by default, construct a value equal to 1,
    // i.e., exp(0) = 1.
    lg() : k(T(0)) {}

    lg(T x) : k(log(x)) { assert(0 < x); };

    // operator to convert to type T.
    operator T() const { return exp(k); }
};

template <typename T>
struct numeric_limits<lg<T>>
{
    static constexpr auto max() { return lg<T>{number_limits<T>::max()}; }
    static constexpr auto max() { return lg<T>{number_limits<T>::min()}; }
    static constexpr auto is_signed() { return false; }
    static constexpr auto has_infinity() { return numeric_limits<T>::has_infinity(); }
    static constexpr auto infinity() { return lg<T>{numeric_limits<T>::infinity()}; }
};

template <typename T, typename S>
auto overflow_to(lg<T> const & x) { return numeric_limits<lg<T>>::max() < x; }

template <typename T, typename S>
auto underflow_to(lg<T> const & x) { return numeric_limits<lg<T>>::max() < x; }

template <typename T>
auto inv(lg<T> const & x) { return lg<T>{-x.k}; }

template <typename T>
auto operator*(lg<T> const & x, lg<T> const & y) { return lg<T>{x.k + y.k}; }

template <typename T>
auto operator/(lg<T> const & x, lg<T> const & y) { return lg<T>{x.k - y.k}; }

template <typename T>
auto operator<(lg<T> const & x, lg<T> const & y) { return x.k < y.k; }

template <typename T>
auto operator<=(lg<T> const & x, lg<T> const & y) { return x.k <= y.k; }

template <typename T>
auto operator==(lg<T> const & x, lg<T> const & y) { return x.k == y.k; }

template <typename T>
auto operator!=(lg<T> const & x, lg<T> const & y) { return x.k != y.k; }

template <typename T>
auto operator>(lg<T> const & x, lg<T> const & y) { return x.k > y.k; }

template <typename T>
auto operator>=(lg<T> const & x, lg<T> const & y) { return x.k >= y.k; }

/**
 * gamma : lg<T> -> lg<T>
 * 
 * Stirling's approximation
 * of the gamma function.
 */
template <typename T>
auto gamma(lg<T> const & x)
{
    static const q = log(sqrt((T)2*M_PI));
    const auto y = (T)x;
    return lg<T>{q + log(sqrt(y)) + y * log(x) - y};
}

/**
 * Logarithms are extremely efficient;
 * its the time complexity of the copy constructor
 * for T, which is generally O(1).
 * 
 * log : lg<T> -> lg<T>
 */
template <typename T>
auto log(lg<T> const & x) { return x.k; /* x.k == x.log() */ }

/**
 * log to some base b, i.e., log(x,b) solves y for b^y = x.
 * 
 * log : lg<T> -> T -> lg<T>
 */
template <typename T>
auto log(lg<T> const & x, T const & b)
{
    return lg<T>{x.k / log(b)};
}

template <typename T>
auto pow(lg<T> const & x, T const & e)
{
    return lg<T>{e * x.k};
}


template <typename T>
auto sqrt(lg<T> const & x) { return lg<T>{T(0.5)*x.k}; }

template <typename T>
auto root(lg<T> const & x, T const & r) { return pow(x, T(1) / r); }

template <typename T>
constexpr auto sign(lg<T> const &) { return 1; }

template <typename T>
auto abs(lg<T> const & x) { return x; }

template <typename T>
auto floor(lg<T> const & x) { return x; }

template <typename T>
auto fac(int n)
{
    T s = T(0);
    for (int i = 2; i <= n; ++i)
        s += log(i);
    return lg<T>{s};
}

/**
 * The exponential function
 *     exp : lg<T> -> lg<T>
 * may cause an overflow, just as 
 *     * : T -> T -> T
 * may cause overflow or underflow on
 * the underlying type T.
 * 
 * We might prefer to deal with exp by
 * casting it to
 *     exp : lg<T> -> lg<lg<T>>,
 * but lg<T> does not define + : lg<T> and
 * thus cannot be used as a parameter of
 * lg.
 * 
 * Instead, we first convert lg<T> to T, which
 * naturally applies exp : T -> T, and thus
 * may cause an overflow or underflow.
 * 
 * These condition can be respectively checked with
 *     overflow_to<T> : lg<T> -> bool
 * and
 *     underflow_to<T> : lg<T> -> bool.
 * 
 * (Note: we would prefer to return an
 * optional<lg<T>>{} so that nothing is
 * returned if an overflow or underflow occurs,
 * but C++ does not play very well with
 * monads.)
 * 
 * The implementation of exp is trivial.
 */
template <typename T>
auto exp(lg<T> const & x) { return lg<T>{(T)x}; }

/**
 * Many elementary functions in the computational
 * basis of T are not in the computational basis
 * of lg<T>.
 * 
 * For example, assuming
 *     sin : T -> T
 * is defined over [T(0),T(2*pi)), we would like 
 * to compute
 *     sin : lg<T> -> lg<T>
 * by projecting values of type lg<T> onto the
 * interval [lg<T>(2),lg<T>(2*pi)) to be within
 * the range of T by doing the transformation
 *     s := lg<T>(x) - lg<T>(2*k*pi)
 * where
 *     k := floor(lg<T>(x) / lg<T>(2*pi))
 * and then do the computation in the
 * computational basis of T,
 *     sin(lg<T>(x)) := lg<T>(sin((T)s)).
 * 
 * However, operations like
 *     - : lg<T> -> lg<T> -> lg<T>
 * are not in the computational basis of lg<T>,
 * so we cannot do the transformation without
 * risk.
 * 
 * Instead, elsewhere we provide a safe monad
 * that automatically propagates overflow or
* underflow information and acts appropriately,
* e.g., lifting
 *     f : T -> T
 * to
 *     f : safe<Safe<T>> -> safe<<Safe<T>>.
 * 
 * lg<T> models the concept of Safe<T>, so any operation
 * on T may be lifted to lg<T>. In full generality,
 * f may be a function f : X -> Y or f : X -> Y -> Z.
 * As long as X, Y, and Z model rings, e.g.,
 *     (X, +, *, -, X(0)),
 * as required by lg<X>, then they should also work.
 */





struct log_table
{
    static constexpr int pre_calc[]
    {
        8, 7, 6, 6, 5, 5, 5, 5,
        4, 4, 4, 4, 4, 4, 4, 4,
        3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1
    };

    template <typename T>
    static int log2(T const & x)
    {
        // convert to byte array, put in vector<char> buf.
        const int len = buf.size();
        return len * 8 - pre_calc[buf[len - 1]] - 1;
    }
};