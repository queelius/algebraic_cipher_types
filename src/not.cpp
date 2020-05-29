#include <iostream>
#include <string.h>
#include <string_view>
#include <string>
#include <boost/program_options.hpp>
#include <vector>
#include <algorithm>

using std::string;
using std::string_view;
using std::cin;
using std::vector;

string const True     = "1";
string const False    = "0";

void output_info(string_view prog)
{
    cout << "Logical-not\n"
         << "-----------\n"
         << "\n"
         << prog << " : Cipher[Bool] -> Cipher[Bool] models logical-not where\n"
         << "Bool is either True or False. The definition is given\n"
         << "by\n"
         << "    " << prog << " False := True,\n"
         << "    " << prog << " True  := False.\n"
         << "\n"
         << "False has the representations \"0\" (without quotes);\n"
         << "everything else represents True.\n"
         << "\n"
         << "If a list of Bool is provided as input, then the not\n"
         << "operator is mapped to the list,\n"
         << "    " << prog << " : List[Bool] -> List[Bool].\n"
         << "\n"
         << prog << " accepts both arguments and standard input.\n";
}

int main(
    int argc,
    char const * argv[])
{
    namespace po = boost::program_options;

    // Declare the supported options.
    po::options_description desc(std::string(argv[0]) + " [options] Bool ...");
    desc.add_options()
        ("help", "output help message")
        ("info", "show detailed info")
        ("in", po::value<vector<string>>()->multitoken(), "one or more Boolean values to logical-and")
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
    
    if (vm.count("in") == 0)
    {
        string v;
        while (cin >> v)
            output_not(v);
    }
    else
    {
        for (auto v : vm["in"].as<vector<string>>())
            cout << (v == False ? True : False) << ' ';
    }

    return EXIT_SUCCESS;

}