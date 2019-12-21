#include <iostream>
#include <stdio.h>
#include <string>

/**
 * 
 * Inputs come from two independent sources:
 * 1. Command-line arguments
 * 2. Standard input (stdin).
 * 
 * We require that there be a file that stores a binary serialization of AHS<X>
 * whose filename is the first argument.
 * 
 * If we tell the shell to execute ahs_contains ahs_file, it will then
 * wait for input from the standard input for the element to test membership
 * in the AHS<X> serialized by ahs_file. This input stream is expected to
 * encode a serialization of values of type X.
 * 
 * We may also pipe the input, e.g., suppose "fruit" is a filename that
 * stores a serialization of AHS<string>. Then,
 *     echo orange | ahs_contains fruit
 * and
 *     ahs_contains fruit orange
 * are equivalent and test whether fruit has element "orange".
 * 
 * If we just execute
 *     ahs_contains fruit
 * then the shell will prompt for input from the standard input
 * (typically keyboard). So, for instance, if the user types "orange<ENTER>"
 * into the console at this prompt, the same behavior as before is exhibited.
 * 
 * On the other hand,
 *     ahs_cardinality fruit,
 *     ahs_fpr fruit,
 * and
 *     ahs_tpr fruit 
 * expect no other input since they represent unary functions.
 * 
 * Executing ahs_contains with no arguments shows the help screen.
 * 
 * Executing ahs_contains fruit "apple" "orange" "almond" outputs to the
 * standard out "1\n1\n0", i.e., you can provide multiple membership
 * queries.
 * 
 * We have another function
 *     logical-and : string[bool] -> string[bool] -> string[bool]
 * which accepts two arguments and emits that are serializations of bool
 * and maps the arguments to their logical-and, i.e.,
 *     and 0 1
 * outputs to the standard out "0".
 * 
 * So, we may perform a Boolean query with
 *     ahs-contains fruit "apple" "almond" | logical-and
 * which returns "0" and
 *     logical-and
 *         ahs-contains fruit apple
 *         logical-not
 *             ahs-contains fruit almond
 * which is equivalent to testing if "apple" and not "almond" is satisfied
 * by fruit, which returns "1".
 * 
 * 
 * 
 * Let s be a FILE stream containing a serialization (FILE) of AHS<X>
 * and let x be a serialization of X, where
 *     contains : AHS<X> -> X -> bool
 * is defined.
 * 
 * Then,
 *     aset_contains s x 
 * returns a serialization of Boolean to the standard out:
 *     std::cout << serialize(contains(deserialize(s), deserialize(x)))
 * 
 */

int main(
    int argc,
    char ** argv)
{
    if (argc == 1)
    {
        std::cout << ""
    }
    for (int i = 0; i < 2; ++i)
        std::cout << argv[i] << '\n';

    std::string el;

    if (!std::cin.eof())
    {
        std::cin >> input;

        std::cout << input << '\n';
    }
}