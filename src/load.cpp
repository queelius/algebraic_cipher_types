#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>

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
    cout    << "Logical-or\n"
            << "-----------\n"
            << prog << " : (Bool,Bool) -> Bool models logical-or where\n"
            << "Bool is either True or False. The definition is given\n"
            << "by\n"
            << "    " << prog << " False False := False, and\n"
            << "    " << prog << " _    _      := True,\n"
            << "\n"
            << "where False has the representations \"0\" (without quotes)\n"
            << "and anything else represents True.\n"
            << "\n"
            << "Logical-or is a monoid with identity False. Thus, it may\n"
            << "reduce a sequence of Bool to Bool, e.g.,\n"
            << "    echo x1 x2 x3 | or --take-all == or(x1,(or(x2,x3))\n"
            << "    echo x1 x2 x3 | or | or == or(x1,(or(x2,x3))\n"            
            << "\n"
            << prog << " accepts command-line arguments or standard input,\n"
            << "e.g., \"echo 0 1 | " << prog << "\" is equivalent to\n"
            << "\"" << prog << " 0 1\".\n"
            << "\n"
            << "Ex. 1: \"" << prog << " 1 0\" outputs \"1\"\n"
            << "Ex. 2: \"" << prog << " 0 0 0 1 0 1 --take 3  \" outputs \"0 1 0 1\"\n";
}

int main(
    int argc,
    char const * argv[])
{
    namespace po = boost::program_options;

    int max;

    // Declare the supported options.
    po::options_description desc(string(argv[0]) + " [options] (Bool, Bool) -> Bool");
    desc.add_options()
        ("help", "output help message")
        ("info", "show detailed info")
        ("reduce-all", po::value<bool>()->default_value(false), "reduce all inputs")
        ("reduce", po::value<int>(&max)->default_value(2), "reduce up to a maximum of arg inputs (and pass the rest unchanged)")
        ("in", po::value<vector<string>>()->multitoken(), "one or more inputs to logical-or")
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
    
    if (vm.count("reduce"))
    {
        max = vm["reduce"].as<int>();
        if (max < 1)
        {
            cerr << "Error: " << argv[0] << " must reduce one or more inputs\n";
            return EXIT_FAILURE;
        }
    }

    int fd;
    fcntl(fd, F_SETFL, O_NONBLOCK);

    vector<string> vs;
    string x;
    while (cin >> x) vs.push_back(x);

    if (vm.count("in") != 0)
    {
        auto tmp = vm["in"].as<vector<string>>();
        for (auto x : tmp)
            vs.push_back(x);
    }

    if (vs.empty())
    {
        cout << "Usage: \n";
        return EXIT_SUCCESS;
    }


    bool result = false;
    for (int i = 0; i < max && i < vs.size(); ++i)
    {
        if (vs[i] != False)
        {
            cout << True << "\n";
            break;
        }
    }
    for (int i = max; i < vs.size(); ++i)
        cout << vs[i] << "\n";
    


}
