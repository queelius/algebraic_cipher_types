/**
 * The sum type is an algebraic type that takes two types X and Y and produces
 * another type X + Y such that values of this type either come from X or Y.
 * Thus, there are |X+Y|=|X|+|Y| values of this type.
 * 
 * Suppose we have a cipher sum type c X + c Y. Then, given an of this type, 
 * it is possible to determine whether it came from X or Y.
 * Indeed, we also know that the overall type is a sum type over these cipher
 * types.
 * 
 * A cipher type c(X+Y) is a cipher over the sum type. We cannot determine the
 * type of the value stored in the sum type. We do not even know that it is
 * a sum type. We only know that it is a cipher type.
 * 
 * The sum type A + B comes with two introduction forms (injections),
 *     inj1: A -> A + B
 * and
 *     inj2: B -> A + B.
 * 
 * The elimination form is case analysis. If e has type A + B and e1 and e2 have
 * type T under the assumptions x: A and y: B respectively, then the term
 *     case e of x -> e1
 *               y -> e2
 * has type T. The sum type corresponds to intuitionistic logical disjunction
 * under the Curry–Howard correspondence.
 */

struct cipher_sum_type
{
    size_t cipher_type_left;
    size_t cipher_type_right;

    size_t cipher_type; // we do not know what the type is
    size_t cipher_value;
}

// suppose we have a function
//     f : cipher_sum_type -> cipher_value

auto get_value(cipher_sum_type x)
{
    return cipher_tag_fn()   
}