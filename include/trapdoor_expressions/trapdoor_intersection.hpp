#include <variant>

template <typename X>
struct trapdoor_universal_set
{
};

template <typename X>
struct trapdoor_empty_set
{
};

template <typename X>
struct trapdoor_intersection
{
    using value_type = X;

    trapdoor_intersection() : value_hash(0), key_hash(0) {}
    trapdoor_intersection(trapdoor_intersection const &)  = default;

    unsigned int value_hash;
    unsigned int key_hash;
};

template <typename X>
using trapdoor_set = variant<trapdoor_universal_set<X>,
                             trapdoor_empty_set<X>,
                             trapdoor_union_set<X>,
                             trapdoor_intersection_set<X>>

template <typename X>
trapdoor_intersection<X> operator*(
    trapdoor_univeral_set<X> const &,
    trapdoor_intersection<X> const & y)
{
    return y;
}

template <typename X>
trapdoor_intersection<X> operator*(
    trapdoor_intersection<X> const & x,
    trapdoor_univeral_set<X> const &)
{
    return x;
}

template <typename X>
trapdoor_intersection<X> operator*(
    trapdoor_intersection<X> const & x,
    trapdoor_empty_set<X> const &)
{
    return trapdoor_empty_set<X>{};
}

template <typename X>
trapdoor_intersection<X> operator*(
    trapdoor_empty_set<X> const & x,
    trapdoor_intersection<X> const &)
{
    return trapdoor_empty_set<X>{};
}

struct tradpoor_visitor
{
    void operator()(Fluid & ) { cout << "fluid\n"; }
    void operator()(LightItem & ) { cout << "light item\n"; }
    void operator()(HeavyItem & ) { cout << "heavy item\n"; }
    void operator()(FragileItem & ) { cout << "fragile\n"; }
};

int main()
{
    std::variant<Fluid, LightItem, HeavyItem, FragileItem> package { 
        FragileItem() };
    std::visit(VisitPackage(), package);
}

template <typename X>
trapdoor_set<X> operator*(
    trapdoor_set<X> const & x,
    trapdoor_set<X> const & y)
{
    // a trapdoor of x of type X with respect to a secret k is different than
    // a trapdoor of x of type X with respect to a secret l. therefore, the
    // intersection of two such sets A and B respectively with secrets k and
    // l is the empty set, i.e., they do not overlap.



    if (x.key_hash != y.key_hash)
        return trapdoor_empty_set<X>{};

    // since xor (^) is assocative and commutative,
    //     * : trapdoor_intersection<X> -> trapdoor_intersection<X> -> trapdoor_intersection<X>
    // is also assocative and commutative.
    return trapdoor_intersection<X>(x.value_hash ^ y.value_hash, x.key_hash);

}

template <typename X>
auto insert(trapdoor<X> const & x, trapdoor_intersection<X> xs)
{
    if (x.key_hash != xs.key_hash)
        throw invalid_argument("mismatched secret keys");

    return trapdoor_intersection<X>(xs.value_hash ^ x.value, x.key_hash);
}


template <typename X1, typename X2>
auto operator*(
    trapdoor_intersection<X1> const & x,
    trapdoor_intersection<X2> const & y)
{
    // values of type X1 do not overlap with values of type X2 so return
    // the empty set.
    return trapdoor_empty_set{};
}

