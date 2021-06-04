


// type-erasure. we erased a cipher<X,N> to cipher<N>.
// we have this safe conversion program that tries
// to convert cipher<N> to cipher<X,N>.
template <typename X, size_t N>
optional<cipher<X,N>> try_convert_to(cipher<N> x)
{
    if (tag_hash(type_id(X), x.s) != x.tag_hash)
        return {};

    return cipher<X,N>{x};
}


template <typename X, typename S, size_t N>
optional<X> try_convert_to(cipher<X,N> x, S s)
{
    // hash with a secret S
    if (hash(s) != x.secret_hash)
        return {};

    //...

    return cipher<X,N>{x};
}


