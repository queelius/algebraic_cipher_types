/***
 * Given a value type A and a parametric vector type v,
 *     v A
 * is a vector of elements of type A with a computational basis
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
 */


template <typename F>
class cipher_vector
{
public:
    using size_type = size_t;
    using value_type = typename F::input_type;
    using key_type = input_type;
    using output_element_type = F::output_type;
    using output_type = list_proxy;
    using hash_type = decltype(hash(declval(input_type)));

    struct unlock_proxy
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

    // could also return cipher (value_type + None), in which case
    // i need to have a cipher if:
    //
    //     if : cipher Bool -> cipher A -> cipher B -> cipher (A + B)
    //     if : cipher True a b := a
    //     if : cipher False a b := b
    auto operator[](cipher_nat const & index) const
    {
        // let x be a cipher_nat
        // if (x < size()) 
        return f_(index);
    }

    auto size() const
    {
        return size_;
    }

private:
    cipher_nat size_;
    // f : query_type -> cipher X -> cipher (Y + Bool)
    // f 'key_query' k := cipher B where B = [k in keys(f)]
    // f 'map_query' k := cipher Y
    F f_;
};


template <typename X>
auto cipher(list<X> const & l)
{

}


template <typename X>
auto cipher(vector<X> const & l)
{
    
}

// could also use concepts and generic containers to pick the appropriate
// types, e.g., if a list, then cipher list, etc


template <typename F>
auto hash(cipher_list<F> const & cl)
{
    return hash(cl.f) ^ hash(cipher_list<F>::tag);
}




