
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
