#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <vector>
#include <string_view>

using std::cout;
using std::string;
using std::cin;
using std::min;
using std::string_view;
using std::vector;
using std::cerr;

string const True     = "1";
string const False    = "0";

void output_info(string const & prog)
{
    cout    << "Cipher map of the logical-and Boolean operation. Models the binary function\n"
            << "(Cipher[Bool],Cipher[Bool]) -> Cipher[Bool] where Cipher[Bool] is the set of\n"
            << "values Cipher[True] + Cipher[False] + Cipher[Nothing].\n"
            << "\n"
            << "The definition is given\n"
            << "by\n"
            << "    " << prog << " Cipher[True] Cipher[True] := Cipher[True], and\n"
            << "    " << prog << " Cipher[False] Cipher[False] := Cipher[False],\n"
            << "    " << prog << " Cipher[False] Cipher[True] := Cipher[False],\n"
            << "    " << prog << " Cipher[True] Cipher[False] := Cipher[False],\n"
            << "    " << prog << " _ _ models a cryptographic hash function onto Cipher[Bool].\n"
            << "\n"
            << prog << " accepts command-line arguments or standard input.\n"
}

int main(
    int argc,
    char const * argv[])
{
    namespace po = boost::program_options;

    // Declare the supported options.
    po::options_description desc(string(argv[0]) + " [options] (Cipher[Bool],Cipher[Bool]) -> Cipher[Bool]");
    desc.add_options()
        ("help", "output help message")
        ("info", "show detailed info")
        ("in", po::value<vector<string>>()->multitoken(), "one or more inputs to logical-and")
        ;

    po::positional_options_description p;
    p.add("in", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("info"))
    {
        output_info(argv[0]);
        return EXIT_SUCCESS;
    }

    if (vm.count("help"))
    {
        cout << desc << "\n";
        return EXIT_SUCCESS;
    }

    bool result = true;
    if (vm.count("in") == 0)
    {
        string v;
        while (cin >> v)
        {
            if (v == False)
                result = false;
        }
    }
    else
    {
        for (auto v : vm["in"].as<vector<string>>())
        {
            if (v == False)
                result = false;
        }
    }

    cout <<  (result ? True : False) << "\n";
    return EXIT_SUCCESS;
}
