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
    cout    << "Logical-and\n"
            << "-----------\n"
            << prog << " : (Bool,Bool) -> Bool models logical-and where\n"
            << "Bool is either True or False. The definition is given\n"
            << "by\n"
            << "    " << prog << " True True := True, and\n"
            << "    " << prog << " _    _    := False,\n"
            << "\n"
            << "where False has the representations \"0\" (without quotes)\n"
            << "and anything else represents True.\n"
            << "\n"
            << "If a sequence of Bool is provided as input, then the\n"
            << "reduction described above is applied,"
            << "    " << prog << " : Bool* -> Bool.\n"
            << "\n"
            << prog << " accepts command-line arguments or standard input,\n"
            << "e.g., \"echo 0 1 | " << prog << "\" is equivalent to\n"
            << "\"" << prog << " 0 1\".\n"
            << "\n"
            << "Ex. 1: \"" << prog << " 1 1\" outputs \"1\"\n"
            << "Ex. 2: \"" << prog << " 0 0 1 1 0 1 --take 5  \" outputs \"0 1\"\n";
}

int main(
    int argc,
    char const * argv[])
{
    namespace po = boost::program_options;

    // Declare the supported options.
    po::options_description desc(string(argv[0]) + " [options] Bool* -> Bool");
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
