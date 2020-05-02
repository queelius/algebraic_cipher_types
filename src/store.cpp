/**
 * A simple key-value (string -> string) store.
 * 
 * Key-value file store has the following simple file format:
 * ---
 * key1     value1
 * key2     value2
 *      .
 *      .
 *      .
 * keyn     valuen
 */

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

void output_info(string_view prog)
{
    cout    << "Key-value store\n"
            << "---------------\n"
            << "\"" << prog << " <key-value-file> <key> <value>\" stores <value> at <key> in <key-value-file>.\n"
            << "\n"
            << prog << " accepts command-line arguments or standard input, e.g.,\n"
            << "all of the following are equivalent:\n"
            << "    echo key value | " << prog << " file\n"
            << "    echo value | " << prog << " file key\n"
            << "    " << prog << " file key value.\n"
            << "\n"
            << "Place arguments in quotes if they include whitespace, e.g.,\n"
            << "    " << prog << " key \"value with spaces\" file.\n";
}


/**
 * Document usage here?
 */

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
