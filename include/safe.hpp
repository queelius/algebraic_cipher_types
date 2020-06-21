/**
 * For some type T with a computational basis F,
 * we define a functor Safe<T> with the same
 * computational basis that, by some mechanism,
 * signals underflows and overflows and propogates
 * this information to any future operations.
 * 
 *  (e.g., T would normally overflow
 * or underflow, but not on Safe<T>, and whenever
 * Safe<T> does overflow or underflow, it is able
 * to detect it).
 * 
 * However, Safe<T> is only defined on {+,-}, whereas
 * T is defined on {+,*,-}.
 * Many elementary functions besides addition are not
 * in the computational basis of lg<T>.
 * 
 * Assuming
 *     f : X -> Y
 * is defined, the functor fmap provides
 *     fmap f : safe<Safe<X>> -> safe<Safe<Y>>.
 * 
 * We do the same for binary functions. Assuming
 *     f : X -> Y -> Z
 * is defined, the functor fmap provides
 *     fmap f : safe<Safe<X>> -> safe<Safe<Y>> -> safe<Safe<Z>>.
 * 
 * These *lifted* functions automatically detect
 * overflows and underflows.
 */

#pragma once

#include <functional>
using std::function;

template <typename T>
struct safe
{
    enum State { valid, overflow, underflow };
    State state; 
    T value;

    safe() : state(valid), value(T::value_type(0)) {}

    safe(T const & x) : value(x)
    {
        if (typename T::overflow_to<T::value_type>(x))
            state = overflow;
        else if (typename T::underflow_to<T::value_type>(x))
            state = underflow;
        else
            state = valid;
    }

    bool invalid() const { return state != valid; }
    bool valid() const { return state == valid; }
    bool overflow() const { return state == overflow; }
    bool underflow() const { return state == underflow; }
    T value() const { return value; }
};

/**
 * Safe models a type that self-detects overflow possibilities.
 * In some cases, it may just know that it's possible, in other
 * cases it may detect exactly when. Depends on the nature of the types
 * involved and the functions being called.
 */
template <template <typename> typename Safe, class X, class Y>
safe<Safe<Y>> fmap(function<Y(X)> f, safe<Safe<X>> x) 
{
    if (x.invalid())
        return safe<Safe<Y>>{ x.state };

    if (typename Safe<X>::overflow_to<X>(x))
        return safe<Safe<Y>>{ overflow };
    
    if (typename Safe<X>::underflow_to<X>(x))
        return safe<Safe<Y>>{ underflow };        

    // we can safely convert x of type Safe<X> to X.

    auto unsafe_x = (X)x;
    // we denote the value of type X unsafe_x, since
    // X is an unsafe type.

    auto y = f(unsafe_x);
    // y is a B, also an unsafe type.
    // if f : A -> B has a problem on unsafe_x,
    // such as overflowing, then y is in an
    // invalid state.

    // Safe<X> cannot detect this in general
    // (although specific implementations may
    // be able to detect invalid states),
    // a precondition on Safe types is that the
    // function to be lifted, f, is total.
    
    // now we convert the unsafe type Y to Safe<Y>.
    return safe<Safe<B>>(y);
}
