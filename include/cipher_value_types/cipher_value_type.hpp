/**
 * A function f : X -> Y is abstractly just a set of tuples,
 *     f := {(x1,y1),(x2,y2),...(xn,yn)}
 * with the notation that
 *     f(x) = y <==> (x,y) in f
 * with the constraint that if (x,y) in f then (x,y') not in f if y'!=y
 * and (x,y) in f => x in X and y in Y.
 * 
 * If f is partial, then for some x in X, there does not exist a y such that
 * (x,y) in f, in which case we say that f(x) is undefined. If a function is
 * defined for every element in the domain, it is denoted a total function.
 * 
 * Even constant values of some type Y may be considered functions of the form
 *     () -> Y,
 * where () denotes some canonical set of size 1. Since it only has a single
 * element, there is no ambiguity with respecto to which element of () is being
 * referred to, and we typically denote the *element* of the set () as () also.
 * 
 * Thus, for instance, a constant c may be understood as a function c : () -> Y
 * with a definition given by c() := c.
 * 
 * Putting aside the complication of side-effects, any program may be understood
 * as computing some function. The computation that models some
 * function is known as a program or algorithm, and conceptually you can just
 * model the computation as a table lookup algorithm with precomputed outputs
 * for every input.
 * 
 * However, since X may be infinite or impractically large to store in a
 * computer memory, a table lookup is typically impractical. Instead we usually
 * use a more space-efficient algorithm to compute the required result.
 * 
 * For instance, suppose f : {1,2,...,n} -> N defined as
 *     f(x) := { x^2 : x in {1,2,...,n} }.
 * 
 * We could just model the function as a table lookup such that x is mapped
 * to the precomputed value f(x) by the table. However, this doesn't scale and
 * it is usually more efficient to model arithmetic operations on number value
 * types and directly implementing the function f as f(x) := x*x.
 * ===
 * 
 * A cipher value type, like any other value type, can also be understood as
 * functions. Thus, if f : X -> Y := { (x1,y1),...,(xn,yn) }, then
 *     (cipher f) : cipher X -> cipher Y :=
 *         { (cipher x1,cipher y1),...,(cipher xn, cipher yn) },
 * where cipher is a morphism, or in category theory a functor of some sort.
 * 
 * 
 */

