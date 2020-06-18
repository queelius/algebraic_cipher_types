#include <cmath>

using std::exp;
using std::log;


/**
 * rate is an abelian group under multiplication. it is not
 * closed under addition,
 *     + : rate x rate -> sum_rate,
 * where sum_rate is a more limited type (but can still be
 * converted to underlying T).
 * 
 * note that we provide an operation * : T x rate -> rate
 * where T is a number [0,infinity), but rate is also
 * a number [0,infinity) which can efficiently provide
 * operations for large numbers (the rate may be extremely
 * large, and most operations will strictly only operate
 * on the exponent of the number with implicit base e).
 * 
 * so, the difference between * : T x rate -> rate
 * and * : rate<T> const & x rate -> rate is one of efficiency.
 */

template <typename T>
struct rate
{
    // we store the exponent of the rate.
    // if w is a rate, then w = w.base()^w.exponent() = exp(k).
    T k;

    rate(T r) : k(log(r)) {};

    auto exponent() const { return k; }
    auto base() const { return exp(1); }

    template <typename T>
    auto to() const { return (T)exp((T)k); }

    static auto identity() { return rate{0}; }

    operator auto() const { return exp(k); }
};

template <typename T>
rate<T> inverse(rate<T> x) { return rate<T>{-x.k}; }

template <typename T>
rate operator*(rate<T> const & x, rate<T> const & y) { return rate{x.k + y.k}; }

template <typename T>
rate operator/(rate<T> const & x, rate<T> const & y) { return rate{x.k - y.k}; }

template <typename T>
bool operator<(rate<T> const & x, rate<T> const & y) { return x.k < y.k; }

template <typename T>
bool operator<=(rate<T> const & x, rate<T> const & y) { return x.k <= y.k; }

template <typename T>
bool operator==(rate<T> const & x, rate<T> const & y) { return x.k == y.k; }

template <typename T>
bool operator!=(rate<T> const & x, rate<T> const & y) { return x.k != y.k; }

template <typename T>
bool operator>(rate<T> const & x, rate<T> const & y) { return x.k > y.k; }

template <typename T>
bool operator>=(rate<T> const & x, rate<T> const & y) { return x.k >= y.k; }


struct rate_sum_expr
{
    rate left;
    rate right;

};
