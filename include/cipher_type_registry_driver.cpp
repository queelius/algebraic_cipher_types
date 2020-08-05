#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <string_view>
#include <optional>
#include <ostream>
#include <fstream>

using std::string;
using std::move;
using std::string_view;
using std::hash;
using std::map;
using std::optional;
using std::map;
/**
 * cipher_type_tagger models the concept of an approximate strong typing for runtime checking.
 *
 * For a cipher constructor, if (approximate) strong typing is desired, then 
 */

#include "cipher_type_tagger.hpp"

int main()
{
    /*
    {
        alex::cipher::cipher_type_registry<0> tagger("secret");

        tagger.insert<int>("int", "secret");
        tagger.insert<double>("double", "secret");
        tagger.insert<float>("float", "secret");
        tagger.insert("not_real", "secret");
        tagger.insert("not_real2", "secret1");

        std::ofstream out("test.txt");

        tagger.serialize(out);
    }
    */

    std::ifstream in("test.txt");
    alex::cipher::cipher_type_registry<0> tagger(in);

    for (auto const & x : tagger)
    {
        std::cout << *x.plaintext("secret") << "\n";
        //std::cout << x.first << " => " << x.second << "\n";
    }


    std::cout << tagger.metadata().queryable_type_info() << "\n";
    std::cout << tagger.metadata().cipher_of_secret() << "\n";
    std::cout << tagger.metadata().magic_bits() << "\n";
    std::cout << tagger.metadata().header() << "\n";


    auto nfo = tagger.type_info(8947580955690856278);
    std::cout << nfo.is_type<int>("secret") << "\n";
    std::cout << nfo.is_any_type("secret") << "\n";
    std::cout << nfo.is_type<bool>("secret") << "\n";
    std::cout << *nfo.plaintext("secret") << "\n";
}