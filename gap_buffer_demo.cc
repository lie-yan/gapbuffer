#include <iostream>
#include "gap_buffer.h"

int main()
{
  std::string s("hello, world");
  dr::gap_buffer<char> gb(s.begin(), s.end());
  for (const auto& c : gb) std::cout << c;
  std::cout << std::endl;
}