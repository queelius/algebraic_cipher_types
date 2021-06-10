#pragma once

#include <memory>
using std::shared_ptr;


/**
 * An abstract function f : X -> Y is just a set of tuples,
 *     f := {(x1,y1),(x2,y2),...(xn,yn)}
 * with the notation that
 *     f(x) = y <=> (x,y) in f
 * and with the constraint that if (x,y) in f, then (x,y') not in f if y'!=y.
 * 
 * If f is partial, then for some x in X, there does not exist a y such that
 * (x,y) in f, in which case we say that f(x) is undefined. If a function is
 * defined for every element in the domain, it is denoted a total function.
 * 
 * Even constant values of some type Y may be considered functions of the form
 *     () -> Y,
 * where Unit is denotes some canonical set of size 1. Since it only has a
 * single element, there is no ambiguity with respecto to which element of ()
 * is being referred to, and we typically denote the *element* of the set ()
 * as () also.
 * 
 * Thus, for instance, a constant c may be understood as a function c : () -> Y
 * with a definition given by c() := c.
 * 
 * Putting aside the complication of side-effects, any program can be understood
 * as computing some function. The computation that models some
 * function is known as an algorithm, and conceptually you can just model the
 * computation as a table lookup, e.g., for a total function of type X -> Y
 * you need a set of tuples of size |X|. Since X may be infinite or
 * impractically to store in a computer memory, this is often impractical, and
 * instead we use a more efficient algorithm to generate the result.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * Type-erased model of the the concept
 * a cipher-if function of type
 *     CipherBool -> A + B
 * where if the input models CipherTrue
 * then a value of type A is returned,
 * otherwise a value of type B is returned.
 * 
 * Note: We may replace CipherBool with
 * NoisyCipherBool, in which case
 * if the input models CipherTrue
 * then a value of type A is returned,
 * if the input models CipherFalse
 * then a value of type B is returned,
 * and otherwise the input is *noise*
 * and some random byte string is
 * returned.
 * 
 * Typically, types A and B model
 * cipher value types, e.g.,
 *     CipherBool -> Cipher<X>+Cipher<Y>,
 * or even
 *     CipherBool -> Cipher<X+Y>.
 */


// A cipher-if of type
//     CipherBool -> T
// models the concept of an if-function
//     bool -> T
// where the adversary is not able to determine whether the output for a given
// input is the result of the input modeling a CipherTrue value or a CipherFalse
// value, assuming:
// (1) The adversary does not know the secret morphism CipherBool -> bool.
// (2) The adversary does not know the if-expression modeling modeled.
// 
// If the adversary may apply the if-function to a CipherBool value and
// determine whether it models CipherTrue or CipherFalse, then it is not a
// cipher-if function of the type
//     CipherBool -> T.
//
// ===
// Removing assumption (2):
//
// Suppose the adversary knows the definition of the if-expression being modeled
// by the cipher-if, e.g.,
//     if true then A else B.
//
// Then, if the adversary observes output A he knows the input was CipherTrue.
// Thus, by removing assumption (2) above, a cipher-if function must satisfy
// a stronger set of conditions.
//
// A stronger condition is a cipher-if of type
//     CipherBool -> CipherT
// such that observing the output does not tell us which value of type T
// is being modeled by the output of type CipherT.
//
// ===
// Removing assumption (1):
//
// Suppose the adversary knows the morphism CipherBool -> bool.
// Then, the function may as well be of type
//     bool -> T
// and we may infer its definition by applying the if-function to true and
// false.
//
// However, what if we want to prevent the adversary from being able to infer
// the definition?
//
// Suppose we have the function
//     bool -> CipherT,
// then applying the if-function to true only and false does not tell us which
// values are being modeled by (if true) and (if false), unless the if-function
// is of the type
//     CipherBool -> CipherBool,
// in wnich case it reduces to bool -> bool.
//
// Note: We could have an if-function of type
//     CipherBool[i] -> CipherBool[j]
// where both types are ciphers of bool, but knowing CipherBool[i] -> bool
// does not reveal information about CipherBool[j].
//
//
// Note that statistical analysis may still be used to estimate whether the
// input models a CipherTrue value or a CipherFalse value. For instance, suppose
// the adversary knows the if-expression being modeled is given by
//     if true then A else B,
// then if a priori P[true] = p, then the probability that an A is observed is
// just p and otherwise 1-p. Therefore, if we have a cipher-if function,
// and we have n values b1,b2,...,bn that model CipherBool, then we can observe
// which values of CipherBool map to A and infer that 
//
// ===
// Removing assumptions (1) and (2):
//
// The adversary now seems to know nearly everything about the if-function.
// 
//     
template <typename T>
class cipher_if
{
public:
    using input_type = cipher<bool>;
    using output_type = T;

    template <typename CipherBool>
    auto operator()(CipherBool e) const
    {
        // A value type that models the concept of a
        // CipherBool can be type-erased to a
        // cipher<bool>.
        //
        // There are an infinite number of ways
        // to model a CipherBool. Two types T and
        // U that model CipherBool are incompatible
        // unless:
        //
        //     (1) There is some way of determining
        //         some identifying feature of their
        //         concrete types, which we call their
        //         type signatures (say a hash or
        //         encryption of the type signature).
        //         Note that if values of types T and U 
        //         were generated from the same
        //         contructors except with differing
        //         secrets, they are different types.
        //
        //     (2) There is some morphism from T to U
        //         and/or U to T, which we call
        //         convert : T -> optinoal<U>.
        //         
        // Of course, on the *trusted* machine, we may
        // just convert T to bool and then, if desired,
        // convert bool to U.
        return if_->eval(e);
    }

    // CipherMap models the concept of
    // a function of type
    //     CipherBool -> A + B
    template <typename CipherMap>
    cipher_if(CipherMap if) : make_shared<CipherMap>{if} {}

private:
    struct concept
    {
        virtual variant<A,B> eval(cipher<bool> e) const = 0;
    };

    shared_ptr<concept const> if_;

    template <typename CipherMap>
    struct model final : concept
    {
        CipherMap if_;

        variant<A,B> eval(cipher<bool> e) const
        {
            // by default, convert(e, e.type_signature()) is
            // just the identity function id(e).
            if (auto g = convert(e, if_.input_type_signature()))
                return if_->eval(g);
        }
    };
};




/**
 * process;
 * 
 * specify input type X
 * specify output type Y
 * 
 * specify (cipher) representations for X (cipher X)
 * specify (cipher) representations for Y (cipher Y)
 * 
 * define function of f: X -> Y
 * 
 * for each x in X:
 *     y := f(x)
 *     [y'] := cipher(y)
 *     ...
 * 
 * 
 * can use a canonical representation of a body
 * e.g., renaming variables to some canonical form,
 * renaming function names to some canoncial form,
 * etc., and then hashing it, to quickly check that
 * two ciphers (say, a cipher of a function) model
 * the same thing. same for types. in this way,
 * we can verify that two, say, cipher bools are
 * of the same type, i.e., same ciphers for true and
 * false.
 * 
 * 
 */


