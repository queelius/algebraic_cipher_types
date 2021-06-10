#pragma once




/**
 * cipher_map_list<F> models
 *     cipher<X> -> cipher<list<cipher<Y>>>
 * where X := F::input_type,
 *       Y := F::output_type,
 * and list models the concept of a sequential list.
 * 
 * f of type F is a cipher map cipher<X> -> cipher<Y> with the characteristic
 * that f(hash x)
 * 
 * We model the list with a proxy that exploits the xor-orbit of F k as being
 * the definition of the list that k maps to.

 * F models a function of type
 *     c X -> c Y
 * where
 *     hash : c X -> hash_type
 *     hash : c Y -> hash_type
 * are defined and
 *     hash(0)
 * is defined and convertible to hash_type.
 * 
 * The cipher map is non-iterable and depending upon the type of cipher
 * map F is, it may an approximate cipher map. Since hash(0) is the sentinel
 * for any list, the random cipher map is constrained to be exact for the
 * sentinel (ideally), otherwise cipher maps may map to arbitrarily large
 * lists. Given that the sentinel is not correctly mapped, the extra length
 * of the list is expected to be 1/|hash_type|, e.g., 2^32, since it is
 * geometrically distributed past that point.
 */



#include <list>
#include <map>
using std::map;
using std::list;



/**
 * computational basis of cipher map list.
 * 
 * it is a cipher map that maps cipher keys to cipher list of cipher values.
 * 
 * the keys are not iterable, but the cipher list for a given key is.
 * it is also a positive random approximate map. first order.
 * 
 * consider:
 * 
 *     trapdoor<X> -> cipher<list<trapdoor<Y>>>
 * 
 * we cipher (obliviously) map trapdoors of type X to ciphers of lists of
 * trapdoors of type Y. We call it a cipher of a list because, prior to
 * being given a trapdoor<X> value, the list in the cipher map cannot
 * a priori be observed. The key "unlocks" it, at which point we see
 * a list<trapdoor<Y>> for the given trapdoor<X> key value. A list by its
 * very nature must be this way, since there is an assumption that we can
 * forward iterate over a list. If it was set, then we could do the
 * contains operation instead, but a list necessarily conveys more info. The
 * best we can do in this case is to hide the list until it's needed, and
 * since the cipher is *noisy*, if we give an invalid key, then it returns
 * a list of of trapdoor<Y> that is pseudo-randomly generated. In this way,
 * an adversary cannot in principle know if the list is valid. We can add
 * an extra cipher set to determine if the key is a member of the map.
 * 
 * consider:
 * 
 *     trapdoor<X> -> trapdoor_set<Y>
 * 
 * note that we could call trapdoor<X> a cipher<X>, but we call it a trapdoor
 * because it cannot be converted back to a value of type X. This is not
 * uncommon for many cipher value types; in almost every case, the only way to
 * do this is through some sort of function that needs to have an entry for
 * each cipher element to map it to a plain element.
 * 
 * we cipher (obliviously) map trapdoors of type X to ciphers of trapdoor sets
 * of type Y where the trapdoor_set<Y> is also non-iterable (that's part of the
 * reason we call it a trapdoor?).
 * 
 */














// cipher map list can model cipher vectors, matrices, seqeunces of properties
// for a key X, etc.
template <typename F>
class cipher_map_list
{
public:
    using size_type = size_t;
    using input_type = typename F::input_type;
    using key_type = input_type;
    using output_element_type = F::output_type;
    using output_type = list_proxy;
    using hash_type = decltype(hash(declval(input_type)));

    struct forward_iterator
    {
        F & f_;
        hash_type h_;
        output_element_type y_;

        auto & operator++()
        {
            h_ ^= hash(y_);
            y_ = f(h_);
            return *this;
        }

        bool operator==(forward_iterator const & rhs) const { return k_ == k; }
        bool operator!=(forward_iterator const & rhs) const { return k_ != k; }
        auto operator*() const { return y_; }
    };

    struct list_proxy
    {
        F & f_;
        hash_type k_;

        constexpr auto sentinal() const
        {
            return hash_type(hash(0));
        }

        auto begin() const
        {
            return forward_iterator{f_, k_, y_(f(k_)};
        }

        auto end() const
        {
            return forward_iterator{f,sentinal()};
        }
    };

    auto operator()(key_type const & key) const
    {
        return list_proxy{f, hash(key)};
    }

    auto key(key_type const & key) const
    {
        return f(key_query,key);
    }

private:
    // f : query_type -> cipher X -> cipher (Y + Bool)
    // f 'key_query' k := cipher B where B = [k in keys(f)]
    // f 'map_query' k := cipher Y
    F f;
};


template <typename X, typename Y>
struct trapdoor_map {};


template <typename X, typename Y>
struct frozen_map {};


template <typename X>
struct trapdoor<std::list<X>> {};

// models the immutable map concept
template <typename X, typename Y>
class trapdoor_map
{
public:
    using size_type = size_t;
    using input_type = trapdoor<X>;
    using key_type = input_type;

    using output_type = Y;
    using value_type = output_type;

    using hash_type = decltype(hash(declval(input_type)));

    auto operator()(key_type const & x) const
    {
        concept_->map(x);
    }

    auto key(key_type const & key) const
    {
        concept_->is_key(key);
    }

private:
    struct concept
    {
        // cipher<bool> is a type-erasure for cipher values of type
        // bool.
        virtual cipher<bool> is_key(X const &) const = 0;
        virtual Y map(X const &) const = 0;
    };

    template <typename F>
    struct model
    {
        cipher<bool> is_key(X const & x) const
        {
            return f.is_key(x);
        }

        Y map(X const & x) const
        {
            return f(x);
        }

        F f;
    };
};









// this should output the data structure for
//     cipher<X> -> cipher<list<cipher<Y>>>
// if there is no way to retrieve the plaintext from cipher<X> or cipher<Y>,
// we denote the type instead by
//     trapdoor<X> -> cipher<list<trapdoor<Y>>>.
// The computational basis of trapdoor<Y> is not sufficient
// to reconstruct Y, so tThe list of trapdoors that trapdoor<X> maps to is also
// a trapdoor since there is no way to convert it to list<Y>. It can only be
// converted to list<trapdoor<Y>>. The computational basis of
// cipher<list<trapdoor<Y>> is iteration of trapdoor<Y> and the computational
// basis of trapdoor<Y>.
//
// So, trapdoor(f) outputs a data structure A that models
//     trapdoor<X> -> trapdoor<list<trapdoor<Y>>>.
// 


template <typename X, typename Y, typename A>
auto cipher(map<K,list<V>> const & f)
{
    for (auto p : f)
    {
        auto key = p.first;
        auto vs = p.second;

        for (auto const & v : vs)
        {

        }
    }
}



template <typename F>
auto hash(cipher_map_list<F> const & f)
{
    return hash(f.f) ^ hash(cipher_map_list<F>::tag);
}




