
# Data abstraction

## Specifying data via interfaces

Every time we decide to represent a certain set of quantities in a particular
way, we are defining a new data type: the data type whose values are those
representations and whose operations are the procedures that manipulate
those entities.

If we decide to change the representation of some data for any reason, we must
be able to locate all parts of a program that are dependent on the
representation. This is accomplished using the technique of data abstraction.

Data abstraction divides a data type into two pieces: an interface and an
implementation. The interface tells us what the data of the type represents,
what the operations on the data are, and what properties these operations
may be relied on to have. The implementation provides a specific represen-
tation of the data and code for the operations that make use of that data
representation.

For instance, suppose we have a data type we call Boolean with the set of
quantities { True, False } with a byte of data where False is represented by
0 (00000000b) and True is represented by 1 (00000001b). Any other byte value is
undefined in this data type. That is to say, Boolean = Byte.

A data type that is abstract in this way is an abstract data type. The rest
of the program, the client of the data type, manipulates the new data only
through the operations specified by the interface. Thus, if wish to change
the representation of the data, all we must do is change the implementation
of the operations in the interface.

For this data type, we define the binary operation
    and : (Boolean, Boolean) -> Boolean,
where
    and(True,True)   := True,
    and(True,False)  := False,
    and(False,True)  := False,
    and(False,False) := False.

We also define the unary operation
    show : Boolean -> Char
wher
    show(True)  := '1' and
    show(False) := '0'.

This obeys the property of the Boolean abstract data type.

The *computational basis* of the Boolean data type also includes
    not : Boolean -> Boolean
and
    or : (Boolean, Boolean) -> Boolean.

## Generic programming

When the client manipulates the values of the data type only through
the procedures in the interface, we say that the client code is representation-
independent, because then the code does not rely on the representation of the
values in the data type.

Taking this further, when we devise algorithms over some generic or parametric
type T and use a set of operations defined in the interface for T, then the
program at least compiles over any data type which defines the required
interface.

Of course, this is not enough. The data type T must also implement the required
operations in a way that obeys the *concept* of the generic type T. This is
a *semantic* distinction.



## Cipher


// cipher_bool<N,M,S1,S2>

template <size_t N, size_t M, size_t S1, size_t S2>
struct cipher_bool
{
    const_iterator operator()(bool x) { ... };


}


See: Random elements to sample cipher_bool true or false.







## Data type
All the knowledge about how the data is represented resides in the
implementation, the most important part of which is the specification of how
the data is represented.

We use the notation $R(d)$ to denote the representation of data $d$.

Consider the data type $Set[X]$ of sets over the universal set $X$; thus,
$Set[X]$ can take on any values in $2^X$ (powerset of $X$).

Suppose the interface consists of
    `emptyset : Set[X]`,
    `singleton : X -> Set[X]`,
    `union : (Set[X], Set[X]) -> Set[X]`, and
    `contains : (Set[X], X) -> B`.
    

A set of procedures will be acceptable as implementations only if they satisfy

    emptyset = R(0)
    singelton R(a) = { a }
    union R(A) R(B) = R(A union B)
    contains R(A) R(x) = I{A}(x)

This is a sufficient computational (or procedural) basis, e.g., to make a
constructor for any set in $2^X$,
    makeset R(a1) ... R(an) = union(
        singleton(R(a1)),
        union(
            singleton(a2),
            union(...,
                union(
                    singleton(R(an),
                    emptyset)))))

    

This specification does not dictate how sets are represented.
It requires only that these procedures produce the specified behavior.
Thus, the procedure emptyset must return the representation of `0`;
the procedure singleton, given the representation of element a, must return the
representation of set `{ a }`;
the procedure union, given the representations of sets A and B, must return
the representation of the union of set A and B;
the procedure contains, given the representation of set A and the
representation of an element x in X, must return I_A(x).

The specification says nothing about the union of two sets over different
univeses, e.g., union : (Set[X],Set[Y])->Set[union(X,Y)], so -- assuming strong
type safety -- under this specification it should result in an error.

Disjoint union, + : (Set[X],Set[Y]) -> Set[X+Y]

has a contains function `contains : X+Y -> B` defined as
`contains (left s) a = ?`
`contains (right s) a = ?`