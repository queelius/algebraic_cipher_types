
/**
 * Let T denote the set of states in a particular Turing machine 
 *     S is the set of symbols, and
 *     D is the set of tape directions {l,r,h}.
 * 
 * 
 * Let the universal Turing machine be a function U of type
 *     U : (T,S -> S,D,T), S* -> S*,
 * where the first argument is the set of (partial) functions and the second
 * argument is the input to the given partial function. If the input is not
 * in the domain of definition of the partial function, then the program
 * implemented by the Turing machine has undefined behavior.
 * 
 * The program to the UTM is encoded by a transition function of type
 *     T,S -> S,D,T
 * where
 *     T is the set of states,
 *     S is the set of symbols, and
 *     D is the set of tape directions {l,r,h}.
 *  
 * In other words, its a relation (T,S,S,D,T) where the first two elements of the relation occur at most once.
 * 
 * There are (|S||D||T|+1)^(|T||S|) possible programs. Typically, S = { 0,1,B } and thus there are
 *     ( 9 |T| + 1 )^( 3 |T| )
 * possible programs. For instance, suppose |T| = 1, then there are 10^3 = 1000 possible programs.
 */

#pragma once

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <tuple>

using std::tuple;
using std::make_tuple;
using std::lower_bound;
using std::upper_bound;
using std::vector;
using std::pair;
using std::string;
using std::sort;

template <typename U>
struct nd_eval
{
    U & u;

    template <typename I>
    auto operator()(I b, I e) const { return b + (u() % distance(b, e)); }
};

struct selector
{
    template <typename I>
    auto operator()(I b, I) const { return b; }
};



template <typename A, typename Q, typename D, typename Selector = selector>
class transition_fn
{
    using alphabet_t = A;
    using state_t = Q;
    using dir_t = D;

    using intput_t = pair<Q,A>;
    using output_t = typename Selector::output_t;

    template <typename I>
    transition_fn(I b, I e, Selector sel) : f_(b,e), sel_(sel)
    {
        sort(f_.begin(),f_.end())
    }

    struct element
    {
        Q cur;
        A rd;
        A wr;
        D dir;
        Q next;
    }

    using relation = vector<element>;   

    struct order
    {
        // partial order; for lookup, we care about first two element types Q
        // and Q in the relation, which defines a relation Q x A x A x D x Q,
        // where x is the n-fold Cartesian product.
        //
        // The selector turns this relation into a functional relation of the
        // type Q x A -> A x D x Q.
        //
        // The selector is given an iterator range
        // of tuple values whose first two component values are equal to
        // the given inputs a and b. By the partial ordering property, these
        // tuples are all int he same equivalence class, and they partition
        // the relation into disjoint subsets. Thus, we provide the selector
        // with the partition related to inputs a and b.
        //
        // The selector gives another degree of freedom in defining how the
        // Turing machine operates. By default, it just returns the first
        // element in the iterator range, but it can do anything else.
        //
        // For instance, the Turing machine may be some extended Turing machine
        // that changes over time, e.g., the selector has a built-in counter
        // and, say, returns the k-th element in the rank on the k-th
        // invocation.
        //
        // More interestingly, the selector may select a random element from the
        // range, thus defining a non-deterministic Turing machine. Many such
        // random walks may return many different outputs, and thus the Turing
        // machine becomes a random program. This may be useful in, say,
        // Monte carlo simulation.
        // 
        // However, it may not actually be a partial function; it could,
        // instead, but a probability distribution, or something else.
        bool operator()(element const & a, element const & b)
        {
            if (a.cur != b.cur)
                return a.cur < b.cur;

            if (a.rd != b.rd)
                return a.rd < b.rd;

            return false;
        }
    };

    auto operator()(Q const & q, A const & a) const
    {
        auto l = lower_bound(f_.begin(),f_.end(),relation{q,a},o_);
        auto u = upper_bound(l,f_.end(),relation{q,a},o_);
        return sel_(l,u);
    }

private:    
    relation f_; 
    order o_;
    Selector sel_;
};




/**
 * Alphabet models the symbol alphabet concept
 *     - blank()
 * 
 * State models the state concept
 *     - halt()
 *     - initial()
 */


/**
 * T models the transition cipher function of type (c Q, c A) -> (c Q, c A,{L,S,R})
 * A models the alphabet
 * Q models the states
 */
template <typename A, typename Q, typenasme T>
class tm
{
public:
    using state_t = Q;
    using alphabet_t = A;
    enum class dir_t { left, stay, right };

    using input_t = vector<A>;
    using output_t = vector<A>;

    tm(T t) : t_(t) {}

    output_t operator()(input_t in)
    {
        constexpr size_t N = 1000;

        int pos = 0;        

        auto cur = A::initial();
        while (cur != A::halt())
        {
            auto rd = in[pos];
            io[pos] = i->wr;
            cur = i->next;
            
            if (i->dir == dirt_t::left)
            {
                --pos;
                if (pos < 0)
                {
                    pos += N;
                    in.insert(in.begin(), N, A::blank());
                }
            }

            if (i->dir == dirt_t::right)
            {
                ++pos;
                if (pos == in.size())
                    in.insert(in.end(), N, A::blank());
            }
        }

        remove_if(in.begin(),in.end(),
            [](A const & x) { return x == A::blank(); });

        return in;
    }

private:
    T t_;
};













/***
 * Given a value type A and a parametric vector type v,
 *     v A
 * is a vector of elements of type A with
 *     get : v A -> I -> A
 *     set : v A -> I -> A -> IO (),
 * where I is an index type.
 * 
 * If we replace A with a cipher value type that models A, denoted c A,
 * then v (c A) = v c A is a vector of cipher values of type A with
 *     get : v c A -> I -> c A
 *     set : v c A -> I -> c A -> IO ().
 * 
 * We can still see that I is a normal index type, which means that an adversary
 * may observe the layout of bits in a value of type v c A and see a sequence of
 * c A, which may be correlated with other observable properties of the system.
 * 
 * Alternatively, if we replace v with a cipher type that models v, denoted c v,
 * with
 *     get : c v A -> c I -> A
 *     set : c v A -> c I -> A -> c IO (),
 * we see that the index type is now a cipher type also. In theory, we could
 * have a parametric type c v with
 *     get : c v A -> I -> A
 *     set : c v A -> I -> A -> c IO (),
 * where c IO () means that the setter, somehow mutates values of type c v A
 * in a theoretically indecipherable way, except that it knows that after
 *     set(xs, i, x),
 * the Boolean expression
 *     get(xs, i, x) == x
 * is true.
 * 
 * A fully general cipher type makes everything about the type indecipherable.
 * 
 * Finally, we would like to compose c A and c v to get the result
 *     c (v (c A)) = c v c A.
 * This is a cipher vector of elements that are cipher values of A with
 *     get : c v c A -> c I -> c A
 *     set : c v c A -> c I -> c A -> c IO (),
 * which gives the outward appearance of noise coming in and out of the get
 * function and noise coming into the set function which causes the
 * data structure c v c A to be randomly mutated.
 * 
 * A reasonable way to implement c vec c A is with a cipher
 * map based on the perfect hash function. It is a little
 * complicated, so let's go into the details.
 * 
 * If v.at(0) = a, we want cv.at(c 0) = ca. This is easy enough
 * for a cipher map, but we must be able to modify the cipher
 * vector also, so it's no longer a function but a mutable
 * data structure.
 * 
 * 
 * 
 */

  T, // transition cipher function of type (c Q, c A) -> (c Q, c A,{L,S,R})

  A, // models the concept of the input alphabet, I subset A

  Q  // models the concept of a set of cipher states
    // the main difference between a plaintext state and a cipher state include:
    //
    //     (1) A cipher state encodes a plaintext state. Let encode : P -> C[P]
    //         encode plaintext P states to cipher C[P] states. Then, encode
    //         is a function that maps states of type P to cipher states of
    //         type C[P].
    //
    //         Let decode : C[P] -> P decode cipher states of type C[P] to plaintext
    //         states of type P. Note that this may be a partial function since
    //         some values in C[P] may not map to any plaintext state. We could consider
    //         the lifted function decode : C[P] -> maybe[P].
    //
    //         We must also have some procedure to convert elements of type P to elements
    //         of type C[P]. We denote this the encode function, with a definition given
    //         by
    //             encode(x) := { p in C[P] | decode(p) = x }.
    //         We see that encode is of type P -> 2^C[P], i.e., each plaintext x may
    //         have multiple cipher values.


/*
 If, over a sample of initial inputs, the distribution of cipher states
              //           for a given equivalence class is flat, we denote this a local homophonic
              //           cipher with respect to the distribution on the initial inputs.
              //
 */ 
              
              //
              //     (2) The entire history of input sequences is a list of I* with a probability
              //         mass function p : sequence[I*] -> [0,1].
              //
              //           Separate input sequences of type I* may be correlated.
              //               P[I(k)* = i*(k) | I(k-1)* = i*(k-1), ..., I(1)* = i*(1)]            
              //           where i{p} is a sequence of of n(p) symbols, (i[p,1],...,i[p,n(p)]).
              //
              //           A single input sequence I*, which has a probability mass p : I* -> [0,1].
              //           The marginal probability p : I -> [0,1] is just the relative
              //           frequency of input symbols across a single sequence and across
              //           separate sequences.
              //           
              //
              //
              //
              //         - if the initial input induces a completely flat distribution across
              //           equivalence classes, this is a global homophonic cipher.
              //     appears uniformly random (if viewing history of them).
              //     



/**
 * T models a transition cipher function of type (c Q, c A) #-> (c Q, c A, {L,S,R})
 * A models the alphabet cipher.
 * Q models the cipher states.
 */
template <typename Q, typename A, typename T>
struct cipher_tm
{
    tm<A,Q,T> tm_;

    using cipher_io_t = tm<A,Q,T>::io_t;
    using cipher_state_t = tm<A,Q,T>::state_t;
    using cipher_alphabet_t = tm<A,Q,T>::alphabet_t;
    using dir_t = tm<A,Q,T>::dir_t;

    auto operator()(cipher_io_t & io)
    {
        return tm_(io);
    }

}


/**
 * If the cipher value types propagating through the cipher system are
 * not the same as a cipher Turing machine, namely for reasons of
 * obfuscation and increasing entropy, then we may consider a composition of
 * the following form.
 * 
 * E models an encoder c A #-> c T where T is the 'native' format.
 * D models a decoder c T #-> c A. Often, c T and c A may be the same and these
 *   two function parameters, E and D, are just the identity.
 */
template <typename Q, typename A, typename T, typename E, typename D>
struct native_cipher_tm
{
    cipher_tm<A,Q,T> tm_;

    using input_t = typename E::cipher_input_t;
    using cipher_t = typename D::cipher_output_t;
    using output_t = typename D::output_t;
    using cipher_state_t = cipher_tm<A,Q,T>::cipher_state_t;
    using cipher_alphabet_t = cipher_tm<A,Q,T>::cipher_alphabet_t;
    using dir_t = cipher_tm<A,Q,T>::dir_t;

    native_cipher_output_t operator()(native_cipher_input_t in)
    {
        
        for (size_t i = 0; i < in.size(); ++i)
        {
            in[i]
        }

        auto out = tm_(in);
        return         
    } 
};






/*
    # Start in state 'b'
    0 * * * b0

    # Turing's state 'b'
    b0 * e r b1
    b1 * e r b2
    b2 * 0 r b3
    b3 * * r b4
    b4 * 0 l b5
    b5 * * l o0

    # Turing's state 'o'
    o0 1 * r o1
    o1 * x l o2
    o2 * * l o3
    o3 * * l o0
    o0 0 0 * q0

    # Turing's state 'q'
    q0 _ 1 l p0
    q0 * * r q1
    q1 * * r q0

    # Turing's state 'p'
    p0 x _ r q0
    p0 e * r f0
    p0 _ * l p1
    p1 * * l p0

    # Turing's state 'f'
    f0 _ 0 l f1
    f1 * * l o0
    f0 * * r f2
    f2 * * r f0
 */
