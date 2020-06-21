Given that type T is a ring,

$$
    (T,+,*,-,T(0))
$$

we define a group

$$
    (\hat{T}, *, ^{-1}, \hat{T}(0))
$$

with homomorphisms $f$ and $g$

$$
    f : T -> \hat{T}
    g : \hat{T} -> T
$$

where if $f$ and g are inverses, e.g.,

$$
    f . g = id,
$$

then $T$ and $\hat{T}$ are isomorphic.
 
$T$ should be a regular type with a total ordering,
e.g., assignment ($=$) and $<$ are defined.
As a result, $\hat{T}$ will also be a regular type
with some ordering relation.

If $f$ is a strictly monotonically increasing
function, $\hat{T}$ will have the same
ordering relation as $T$ on values in the range
of $T$. Moreover, $\hat{T} will extend that
relation to values outside the range of $T$.
 
If $f$ is not injective, then elements
mapped to the same value by $f$ are
in the same equivalence class, i.e.,
if $x$ and $y$ are elements of $T$, then
we may denote their class by $[x]$ or $[y]$
and

$$
    [x] == [y].
$$

In this case, $T$ is not a Since T has a total order, there is still
a partial order 
ordering relation applies to equivalence
classes, then  and
extend<T> only has a partial order (even
though )

This only occurs when f is a monotonically
increasing function with respect to
     < : T x T -> bool.

The fact that a value of type extend<T> must
initially be constructed from a value of type
T means that these constructions are lossless.
However, since extend<T> is a group with
opeations, namely *, a value of type
extend<T> may not be a value in T. Typically,
the value extend<T> would conceptually map to
would be outside the range of T, e.g., either
too small or too large.

We also can expect, depending on f and g, some
independent sources of loss, e.g., if f
introduces some minor loss due to rounding
or truncation. 

The operation
    * : extend<T> x extend<T> -> extend<T>
is defined as
    a * b := f ( g a + g b ),
which is associative by the associativity of +.

The inverse operation
    ^(-1): extend<T> -> extend<T>
is defined as
    a^(-1) := f ( -g(a) ).

Finally, the identity
    extend<T>(T(0))
must be given by
    a * a^(-1) := f ( g a + g ( a^(-1) ),
thus we see that g must satisfy
    ( g a^(-1) ) + (g a) = T(0),
i.e., f and g must map inverses to inverses
and f and g must map identity to identity.

extend<T>, as suggested by the name, is intended to make computations
more precise, faster, or less suspetible to underflows or overflows,
 * than T over a more restricted set of operations, where the computational
 * basis is {+, ^(-1)}.
 * 
 * Example:
 * 
 * When dealing with very large numbers or very small numbers inn numerical
 * computation, it is frequently faster and precise to work with logarithms
 * of numbers and converting to objective values only when needed. This
 * allows multiplication to be as simple and fast as addition, and
 * avoids most intermediate steps that would otherwise result in underflows
 * or overflows even when the final result is within the range of T.
 * If the final result is not within the range of T, which could be checked,
 * other steps may be taken, e.g., using a slow bignum library.
 */

