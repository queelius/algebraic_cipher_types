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

vector<string> const True =
{
    "0011", "1010",
    "1000", "1110",
}

vector<string> const False =
{
    "0001", "1010",
    "1011", "1111",
}

void output_info(string const & prog)
{
    cout    << "A noisy cipher of Boolean logical-and operation.\n"
            << "\n"
            << "This consists of two orthogonal parts, the noise and the cipher.\n"
            << "\n"
            << "    (1) noisy<X> is a set consisting of signals with values of\n"
            << "        type X and a noisy value Noise that has no meaning. It\n"
            << "        is a monad such that any function\n"
            << "            f : X -> Y\n"
            << "        is lifted to\n"
            << "            noisy<f> : noisy<X> -> noisy<Y>\n"
            << "        such that if x is a signal, then noisy<f> x = f x and\n"
            << "        noise<f> Noise is a discrete uniform distribution onto\n"
            << "        the *representations* of values of noisy<Y>.\n"
            << "\n"
            << "        For instance, if noisy<Y> has prefix-free codes, then\n"
            << "        noisy<f> Noise will map to some value y in noisy<Y> with\n"
            << "        probability 2^-|encode(y)|.\n"
            << "\n"
            << "        If we want Noise to map to Noise, then the\n"
            << "        code(s) for Noise should be either short or many, i.e.,\n"
            << "        suppose Noise has been assigned the set of codes {0,10},\n"
            << "        then P[noisy<f> Noise = Noise] = 1/2+1/4=3/4.\n"
            << "\n"
            << "        We may deconstruct a noisy value type noisy<X> with the\n"
            << "        homomorphism signal : noisy<X> -> X, which only succeeds\n"
            << "        if noisy<X> is a signal.\n"
            << "\n"
            << "        An algebraic data type that includes a noisy type is not\n"
            << "        neccessarily a noisy algebraic data type since the\n"
            << "        monadic property is not guaranteed, e.g., if f x y := g y\n"
            << "        then signal (f Noise y) = g(y), i.e., a noisy component\n"
            << "        does not generate random output.\n"
            << "\n"
            << "        More formally, (noisy<X>,Y) is not isomorphic to\n"
            << "        noise<(X,Y)> since the cardinality of (noisy<X>,Y) is\n"
            << "        (|X|+1)|Y| while the cardinality of noise<(X,Y)> is\n"
            << "        |X||Y|+1.\n"
            << "\n"
            << "    (2) cipher<bool> is a cipher of the value type bool. A\n"
            << "        classical bool is a set bool := {true,false} with a set\n"
            << "        of well-defined logical operators like\n"
            << "            not : bool -> bool\n"
            << "        and\n"
            << "            and : bool -> bool -> bool.\n"
            << "\n"
            << "        A cipher of bool is a homomorphism from {true,false} to\n"
            << "        cipher({true,false}) where true and false map to disjoint\n"
            << "        sets cipher(true) and cipher(false).\n"
            << "\n"
            << "An interesting case occurs when both the composite\n"
            << "algebraic type and the types it is composing are\n"
            << "noisy types, such as (X,Y) being mapped to\n"
            << "noisy<(noisy<X>,noisy<Y>)>. In this case, we lift\n"
            << "    f : X -> Y -> Z\n"
            << "to\n"
            << "    noisy<f> : noisy<(noisy<X>,noise<Y>)> -> noisy<Z>\n"
            << "such that\n"
            << "    noise<f> Noise Noise\n"
            << "and\n"
            << "    noise<f> Noise\n"
            << "are both the discrete uniform distribution of representations\n"
            << "of values of noisy<Z>.\n"
            << "\n"
            << "Thus, cipher<noise<bool>> is a cipher of noisy Boolean values.\n"
            << "\n"
            << "Any function  cipher<noise<bool>> then must have the following\n"
            << "properties:\n"
            << "    (a) "
            << "Models the binary function\n"
            << "    noisy<cipher<bool>> -> noisy<cipher<bool>> -> noisy<cipher<bool>>.\n"
            << "\n"

            << "\n"
            << "Thus, noisy<cipher<bool>> "
            << "    " << prog << " cipher<noise> cipher<noise> is a random variable with support noisy<cipher<bool>>,\n"
            << "    " << prog << " cipher<true> cipher<true> := cipher<true>,\n"
            << "    " << prog << " cipher<false> cipher<true> := cipher<true>,\n"
            << "    " << prog << " cipher<true> cipher<false> := cipher<true>,\n"
            << "    " << prog << " cipher<false> cipher<false> := cipher<true>,\n"
            
            << "\n"
            << prog << " accepts command-line arguments or standard input.\n"
}

int main(
    int argc,
    char const * argv[])
{
    namespace po = boost::program_options;

    // Declare the supported options.
    po::options_description desc(string(argv[0]) + " [options] noisy<cipher<bool>> -> noisy<cipher<bool>> -> noisy<cipher<bool>>");
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
