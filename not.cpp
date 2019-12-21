#include <iostream>
#include <string.h>
#include <string_view>
#include <string>
#include <boost/program_options.hpp>
#include <vector>
#include <algorithm>

std::string const True     = "1";
std::string const False    = "0";

void output_not(std::string_view v)
{
    std::cout << (v == False ? True : False) << ' ';
}

void output_equiv(std::string_view v)
{
    std::cout << (v == False ? False : True) << ' ';
}

void output_info(std::string_view prog)
{
    std::cout   << "Logical-not\n"
                << "-----------\n"
                << "\n"
                << prog << " : Bool -> Bool models logical-not where\n"
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
                << prog << " accepts both arguments and standard input,\n"
                << "e.g., \"echo 0 | " << prog << "\" is equivalent to\n"
                << "\"" << prog << " 0\".\n"
                << "\n"
                << "Usage: " << prog << " Bool ...\n"
                << "\n"
                << "Ex. 1: \"" << prog << " 0 1\" outputs \"1 0\"\n"
                << "\n"
                << "Ex. 2: \"" << prog << " 1 1 0 0 1 1 --take 2 --drop 2 \" outputs \"1 1\"\n"
                << "\n"
                << "Ex. 3: \"" << prog << " --drop 2 --in 1 1 0 0 1 1 \" outputs \"1 1\"\n"
                << "\n"
                << "Ex. 4: \"echo 1 0 | " << prog << " --take 1 --pass | " << prog << " --skip 1 --take 1 --pass | ./and | " << prog << "\n"
                << "implements logical-or, which maps the input \"1 0\" to  \"1\".\n";
}

int main(
    int argc,
    char const * argv[])
{
    namespace po = boost::program_options;

    int max = std::numeric_limits<int>::max();
    int skip;
    
    // Declare the supported options.
    po::options_description desc(std::string(argv[0]) + " [options] Bool ...");
    desc.add_options()
        ("help", "output help message")
        ("take", po::value<int>(&max),
            "take up to a maximum of arg inputs")
        ("skip", po::value<int>(&skip)->default_value(0),
            "skip the first arg inputs")
        ("info", "show detailed info")
        ("pass", "pass skipped or untaken inputs to the output unchanged in the original order")
        ("in", po::value<std::vector<std::string>>()->multitoken(), "one or more Boolean values to logical-and")
        ;

    po::positional_options_description p;
    p.add("in", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    bool pass = vm.count("pass") != 0;

    if (vm.count("info"))
    {
        output_info(argv[0]);
        return EXIT_SUCCESS;
    }

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return EXIT_SUCCESS;
    }
    
    if (vm.count("take"))
    {
        max = vm["take"].as<int>();
        if (max < 1)
        {
            std::cerr << "Error: " << argv[0] << " must take 1 or more inputs, i.e., \"" << argv[0] << "--take n Bools ... \", n > 0.\n";
            return EXIT_FAILURE;
        }
    }

    if (vm.count("skip"))
    {
        skip = vm["skip"].as<int>();
        if (skip < 0)
        {
            std::cerr << "Error: " << argv[0] << " cannot skip negative inputs, i.e., \"" << argv[0] << "--skip n Bools ... \", n >= 0.\n";
            return EXIT_FAILURE;
        }
    }

    if (vm.count("in") == 0)
    {
        std::string v;
        if (skip != 0)
        {
            while (skip-- != 0 && std::cin >> v)
                if (pass) output_equiv(v);
        }
        
        int i = 0;
        while (i++ < max && std::cin >> v)
            output_not(v);

        if (pass)
        {
            while (std::cin >> v)
                output_equiv(v);
        }
    }
    else
    {
        auto values = vm["in"].as<std::vector<std::string>>();

        if (skip != 0 && pass)
        {
            for (int i = 0; i < skip; ++i)
                output_equiv(values[i]);
        }

        int i = skip;
        for ( ; i < std::min(max+skip, (int)values.size()); ++i)
            output_not(values[i]);

        if (pass)
        {
            for ( ; i < (int)values.size(); ++i)
                output_equiv(values[i]);
        }
   }

}