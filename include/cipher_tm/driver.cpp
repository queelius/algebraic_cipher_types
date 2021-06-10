#include <iostream>
#include <string>
#include <variant>
//#include <hash_map>

std::variant<int,std::string>
test(bool x)
{
  if (x) return std::string("test");
  else   return 2;
}

int main()
{
  std::hash<int> h;
  std::hash<const char*> h2;
  std::cout << h(10) << std::endl;
  std::cout << h2("test") << std::endl;

  /*std::cout << sizeof(size_t) << std::endl;
  std::cout << std::numeric_limits<size_t>::max() << std::endl;
  std::cout << std::numeric_limits<long double>::max() << std::endl;

  std::cout << sizeof(long double) << std::endl;*/

  using int128_t = __int128;
  int128_t xxx = 1;
  xxx = xxx <<= 127;
  printf("%d",xxx);


  auto xx = static_cast<long double>(1e-50) / std::numeric_limits<size_t>::max();
  std::cout << xx << std::endl;
}