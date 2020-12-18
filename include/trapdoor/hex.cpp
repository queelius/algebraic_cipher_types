


#include <iostream>
#include <sstream>
#include <iomanip>
#include <string_view>

unsigned int to_unsigned(std::string_view hex, int base=10)
{
    return strtoul(hex.data(),nullptr,base);
}

std::string to_hex_str(unsigned int x, int width=0, char fill='0')
{
    std::stringstream s;
    s << std::setw(width) << std::setfill(fill) << std::hex << x;
    return s.str();
}

int main()
{
    uint8_t x[] = {0xff, 0x00, 0x01, 0x20};
    uint32_t y = 0xff000120;

    for (int i = 0; i < 4; ++i)
        std::cout << to_hex_str(to_unsigned(to_hex_str(x[i]),16),2);
    std::cout << "\n";

    std::cout << std::hex << y << "\n";


    return 0;
}