
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

using std::lower_bound;
using std::upper_bound;
using std::vector;
using std::pair;
using std::string;
using std::sort;

/**
 * Alphabet models the symbol alphabet concept
 *     - blank()
 * 
 * State models the state concept
 *     - halt()
 *     - initial()
 */
template <typename Alphabet, typename State>
struct utm
{
    using state_t = State;
    using alphabet_t = Alphabet;
    enum class dir_t { left, stay, right };

    struct relation
    {
        State cur;
        Alphabet rd;
        Alphabet wr;
        dir_t dir;
        State next;
    };

    using partial_fn = vector<relation>;
    using io_t = vector<Alphabet>;

    struct order
    {
        // partial order; we only care about first two elements in the relation,
        // which defines a partial function over (T,S) -> (S,D,T).
        //
        // this sorting strategy minimizes functional evaluation complexity,
        // N(log |T| + log |S|), doing the minimal work necessary to get the
        // vector of tuples to have the necessary structure.
        bool operator()(relation const & a, relation const & b)
        {
            if (a.cur != b.cur)
                return a.cur < b.cur;

            if (a.rd != b.rd)
                return a.rd < b.rd;

            return false;
        }
    };

    template <typename U>
    struct nd_eval
    {
        U & u;

        template <typename I>
        auto operator()(I begin, I end) const { return lb + (u() % distance(lb, ub)); }
    };

    struct eval
    {
        template <typename I>
        auto operator()(I begin, I end) const { return begin; }
    };


    template <typename Eval>
    bool run_once(
        io_t & io,
        partial_fn const & f,
        Eval & eval)
    {
        constexpr size_t N = 1000;

        int pos = 0;
        sort(f.begin(), f.end(), order{});

        Alphabet cur = Alphabet::initial();
        while (cur != halt)
        {
            auto rd = io[pos];

            auto i = retrieve_one(f.begin(),f.end)
            auto lb = lower_bound(f.begin(),f.end(),relation{cur,rd},order{});
            if (lb == f.end())
                return false;
            auto ub = upper_bound(lb,f.end(),relation{cur,rd},order{});
            auto i = eval(lb,ub);

            io[pos] = i->wr;
            cur = i->next;
            
            if (i->dir == dirt_t::left)
            {
                --pos;
                if (pos < 0)
                {
                    pos += N;
                    io.insert(io.begin(), N, Alphabet::blank());
                }
            }

            if (i->dir == dirt_t::right)
            {
                ++pos;
                if (pos == io.size())
                    io.insert(io.end(), N, Alphabet::blank());
            }
        }

        remove_if(io.begin(),io.end(),
            [](Alphabet const & x) { return x == Alphabet::blank(); });

        return io;
    };
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












/***
 * Can we replace {L,S,R} with c {L,S,R}? We use the direction to increment or decrement
 * a position (for the IO sequence).
 * 
 * So, given a plaintext alphabet vector of type
 *     vec A
 * with
 *     at : vec A -> index -> A
 *     set : vec A -> index -> A -> IO (),
 * we replace it with a cipher alphabet vector of type
 *     c vec c A
 * with
 *     at : c vec c A -> c index -> c A
 *     set : c vec c A -> c index -> c A -> c IO ()
 * 
 * This is the ideal situation, at least. It is possible
 * that instead of c vec c A, we will have a more revealing
 * vec c A, which is just a plaintext vector whose elements
 * are ciphers with 
 *     at : vec c A -> index -> c A
 *     set : vec c A -> index -> c A -> IO ().
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

template<
  typename T, // transition cipher function of type (c Q, c A) -> (c Q, c A,{L,S,R})

  typename I, // models the concept of the input alphabet, I subset A

  typename A, // models the concept of the tape alphabet

  typename Q  // models the concept of a set of cipher states
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
              //         the lifted function encode : C[P] -> maybe[P].
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
>
struct cipher_tm
{
  struct tape
  {
    std::vector<A>;

    void set_input(std::vector<I> in)
    {

    }
  }
  
  using TapeAlphabet = A;
  using InputAlphabet = I;
  using TransitionFn = T;
  using TapePosition = int;

  Q q;
  T t;

  void run(I input)
  {
    Tape tape;
    TapePosition p = 0;
  };
};
