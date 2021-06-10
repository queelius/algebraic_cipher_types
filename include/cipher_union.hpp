/**
 * The sum type is an algebraic type that takes two types X and Y and produces
 * another type X + Y such that values of this type either come from X or Y.
 * Thus, there are |X+Y|=|X|+|Y| values of this type.
 * 
 * Suppose we have a  cipher sum type
 *     (cipher (X+Y)).
 * 
 * Given an element of a cipher (X+Y)), it is possible to determine whether it
 * came from X or Y. This means that we may retrieve the type of the cipher
 * value stored in the sum type, which means it is actually of type
 *     (cipher X) + (cipher Y),
 * where any value of this type is tagged as a sum type, and the tag of the
 * actual value, (cipher X) or (cipher Y), is also stored.
 * 
 * However, what if we insist on a type (cipher (X+Y))? We should be able to
 * see what  the sum type is, but when we retrieve the type of the value stored
 * in the sum type, the untrusted system must not be able to determine if
 * it is (cipher X) or (cipher Y). That is to say, we must cipher (cipher X)
 * and cipher (cipher Y) and return those.
 * 
 * 
 * 
 * The sum type A + B comes with two introduction forms (injections),
 *     inj1: A -> A + B
 * and
 *     inj2: B -> A + B.
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