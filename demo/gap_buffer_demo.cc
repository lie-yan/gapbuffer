#include <iostream>
#include "gap_buffer.h"

template <typename T>
void print_gb(dr::gap_buffer<T> const& gb)
{
  for (const auto& c : gb) std::cout << c;
  std::cout << std::endl;
}

int main()
{
  std::string s("hello, world");
  dr::gap_buffer<char> gb(s.begin(), s.end());
  print_gb(gb);

  for (auto i = gb.crbegin(); i != gb.crend(); ++i)
    std::cout << *i;
  std::cout << std::endl;

  gb.erase(gb.begin(), gb.begin() + 2);

}