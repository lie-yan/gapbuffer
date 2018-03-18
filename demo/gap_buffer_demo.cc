#include <iostream>
#include "gap_buffer.h"

template<typename T>
void print_gb(dr::gap_buffer<T> const& gb)
{
  for (const auto& c : gb) std::cout << c;
  std::cout << std::endl;
}

class A {
public:
  A(char ch = 'd')
      : _ch(ch)
  {
//    std::cout << "default constructor\n";
  }
  A(const A& a)
      : _ch(a._ch)
  {
//    std::cout << "copy ctor\n";
  }
  A(A&& a)
      : _ch(a._ch)
  {
//    std::cout << "move constructor\n";
  }

  A& operator =(const A& a)
  {
    _ch = a._ch;
//    std::cout << "copy assign\n";
    return *this;
  }

  A& operator =(const A&& a)
  {
    _ch = a._ch;
//    std::cout << "move assign\n";
    return *this;
  }

  friend
  std::ostream& operator <<(std::ostream& os, const A& a)
  {
    os << a._ch;
    return os;
  }
private:
  char _ch;
};

int main()
{
  std::string s("hello, world");
  dr::gap_buffer<char> gb(s.begin(), s.end());
  print_gb(gb);

  for (auto i = gb.crbegin(); i != gb.crend(); ++i)
    std::cout << *i;
  std::cout << std::endl;

  gb.erase(gb.begin(), gb.begin() + 2);

  gb = {'1', '2', '3'};
  print_gb(gb);

  gb.assign(s.begin(), s.end());
  print_gb(gb);

  gb.assign({'a', 'b', 'c'});
  print_gb(gb);

  auto it = gb.insert(gb.begin(), 'd');
  print_gb(gb);

  gb.insert(it, 5, 'e');
  print_gb(gb);

  gb.pop_back();
  print_gb(gb);

  gb.resize(3);
  print_gb(gb);

  gb.resize(10, 'x');
  print_gb(gb);

  dr::gap_buffer<A> gba;

  gba.insert(gba.begin(), A('x'));

  print_gb(gba);

  gba.push_back(A('b'));

  gba.emplace(gba.end(), 'z');

  print_gb(gba);

  gba.insert(gba.begin(), 100, A('x'));
  gba.erase(gba.begin(), gba.begin() + 80);
  std::cout << gba.capacity() << std::endl;

  gba.shrink_to_fit();
  std::cout << gba.capacity() << std::endl;

  print_gb(gba);

  dr::gap_buffer<char> gb1 = {'a', 'b', 'c'}, gb2 = {'c', 'd', 'e'};
  std::cout << (gb1 < gb2) << std::endl;

}