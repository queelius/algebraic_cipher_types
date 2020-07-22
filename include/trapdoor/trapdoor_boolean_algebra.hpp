template <typename X, size_t N>
struct trapdoor_boolean_algebra
{
    using value_type = X;

    trapdoor_boolean_algebra() :
        value_hash(0),
        key_hash(0)
    {
        // makes the empty set
    }

    trapdoor_boolean_algebra(trapdoor_boolean_algebra const &) = default;

    array<char,N> value_hash;
    array<char,4> key_hash;
};

template <typename X, size_t N>
auto make_empty_trapdoor_set()
{
    return trapdoor_boolean_algebra<X,N>();
}

/**
 * The disjoint union operation is a partial function that is only defined
 * when the argument sets are disjoint (it is a dependent type). If they are
 * not disjoint, the operation has undefined behavior.
 */
template <typename X, size_t N>
auto operator+(
    trapdoor_boolean_algebra<X,N> const & x,
    trapdoor_boolean_algebra<X,N> const & y)
{
    if (x.key_hash != y.key_hash)
        throw invalid_argument("secret key mismatch");

    return trapdoor_boolean_algebra<X>(
        x.value_hash | y.value_hash,
        x.key_hash);
}

template <typename X, size_t N>
auto operator!(
    trapdoor_boolean_algebra<X,N> const & x)
{
    return trapdoor_boolean_algebra<X>(
        ~x.value_hash,
        x.key_hash);
}

template <typename X, size_t N>
auto operator*(
    trapdoor_boolean_algebra<X,N> const & x,
    trapdoor_boolean_algebra<X,N> const & y)
{
    if (x.key_hash != y.key_hash)
        throw invalid_argument("secret key mismatch");

    return trapdoor_boolean_algebra<X>(
        x.value_hash & y.value_hash,
        x.key_hash);
}


template <typename X, typename Y, size_t N>
auto disjoint_union(
    trapdoor_boolean_algebra<X,N> const & x,
    trapdoor_boolean_algebra<Y,N> const & y)
{
    if (x.key_hash != y.key_hash)
        throw invalid_argument("secret key mismatch");

    return trapdoor_boolean_algebra<variant<X,Y>>(
        x.value_hash | y.value_hash,
        x.key_hash);
}


template <typename X>
approximate_bool empty(trapdoor_boolean_algebra<X> const & xs)
{
    auto b = std::all_of(xs.begin(),xs.end(),[](char x) { return x == 0; });
    return approximate_bool{b,0.5};
}


template <typename X, size_t N>
approximate_bool contains(
    trapdoor<X,N> const & x,
    trapdoor_boolean_algebra<X,N> const & xs)
{
    if (x.key_hash != xs.key_hash)
        throw invalid_argment("secret key mismatch");

    return approximate_bool{b, .5};
}

template <typename X>
approximate_bool operator<=(
    trapdoor_boolean_algebra<X> const & x,
    trapdoor_boolean_algebra<X> const & y)
{
    auto b = std::all_of(xs.begin(),xs.end(),[](char x) { return x == 0; });
    return approximate_bool{b, .5};
}

template <typename X>
approximate_bool operator==(
    trapdoor_boolean_algebra<X> const & x
    trapdoor_boolean_algebra<X> const & y)
{
    auto b = std::all_of(xs.begin(),xs.end(),[](char x) { return x == 0; });
    return approximate_bool{b, .5};
}


template <typename X, size_t N>
auto hash(trapdoor_boolean_algebra<X,N> const & x)
{
    return x.value_hash ^ x.key_hash ^ hash(typeid(X))
}