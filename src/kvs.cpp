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
#include <string_view>
#include <algorithm>
#include <boost/program_options.hpp>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <set>

using std::string;
using std::pair;
using std::cout;
using std::string;
using std::cin;
using std::min;
using std::string_view;
using std::vector;
using std::set;
using std::cerr;

void output_info(string_view prog)
{
    cout    << "Key-value store\n"
            << "---------------\n"
            << "\"" << prog << " --key-value-file <file> --key <key> <value>\" stores <value> at <key> in <key-value-file>.\n"
            << "\"" << prog << " --key-value-file <file> --keys <keys>\" retrieves values corresponding to each key in <keys> in <key-value-file>.\n"
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

    string key_value_file;
    vector<string> keys;
    string key, value;

    // Declare the supported options.
    po::options_description desc(string(argv[0]) + " [options]");
    desc.add_options()
        ("help", "show help")

        ("info", "show detailed info")

        // also allow env variables or config files to be used
        ("key-value-file", po::value<string>(&key_value_file), "key-value file store")

        ("pair", "show as key-value pairs")

        ("all", "show all values")

        ("key", po::value<string>(&key), "key to set (or look up)")

        ("value", po::value<string>(&value), "value to set for corresponding key")

        ("keys", po::value<vector<string>>(&keys), "keys to lookup the corresponding values for")

        ;

    po::positional_options_description p;
    p.add("key-value-file", 1);
    p.add("key", 1);
    p.add("value", 1);

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

    if (key_value_file.empty())
    {
        cerr << "No key-value-file specified.\n";
        return EXIT_FAILURE;
    }

    if (vm.count("key") && vm.count("value"))
    {
        std::fstream f(key_value_file); // use option std::ios_base::binary if necessary
        
        //f.seekp(position_of_data_to_overwrite, std::ios_base::beg);
        //f.write(my_data, size_of_data_to_overwrite);     

        string k;
        while (f >> k)
        {
            if (k == key)
            {
            }
        }
    }
    else if (vm.count("key")) // && vm.count("value") == 0
    {
        std::ifstream f(key_value_file); // use option std::ios_base::binary if necessary
        
        string k;
        while (f >> k)
        {
            if (k == key)
            {
                string v;
                f >> v;
                if (vm.count("pair"))
                    cout << k << "\t" << v << "\n";
                else
                    cout << v << "\n";
                break;
            }
        }
    }
    else if (vm.count("all"))
    {
        std::ifstream f(key_value_file);
        string k, v;
        while (f >> k)
        {
            f >> v;
            if (vm.count("pair"))
                cout << k << "\t" << v << "\n";
            else
                cout << v << "\n";
        }
        f.close();
    }
    else if (vm.count("keys"))
    {
        std::ifstream f(key_value_file);
        string k, v;
        while (f >> k)
        {
            if (set<string>(keys.begin(),keys.end()).count(k) != 0)
            {
                f >> v;
                if (vm.count("pair"))
                    cout << k << "\t" << v << "\n";
                else
                    cout << v << "\n";
            }
        }
        f.close();
    }
    else
    {
        cout << "No arguments specified!\n\n"
                << desc << "\n";
    }

    return EXIT_SUCCESS;
}
