#include <iostream>
#include "gap_buffer.h"

template<typename T>
void print_gb(std::string const& s, dr::gap_buffer<T> const& gb) {
  std::cout << s << ": \"";
  for (const auto& c : gb) std::cout << c;
  std::cout << "\"";
  std::cout << std::endl;
}

class A {
public:
  A(char ch = 'd')
      : _ch(ch) {
//    std::cout << "default constructor\n";
  }
  A(const A& a)
      : _ch(a._ch) {
//    std::cout << "copy ctor\n";
  }
  A(A&& a)
      : _ch(a._ch) {
//    std::cout << "move constructor\n";
  }

  A& operator =(const A& a) {
    _ch = a._ch;
//    std::cout << "copy assign\n";
    return *this;
  }

  A& operator =(const A&& a) {
    _ch = a._ch;
//    std::cout << "move assign\n";
    return *this;
  }

  friend
  std::ostream& operator <<(std::ostream& os, const A& a) {
    os << a._ch;
    return os;
  }
private:
  char _ch;
};

// We will replace the following code with a googletest case later. 
int main() {
  using namespace dr;

  gap_buffer<char> gb0(10);
  print_gb("gb0", gb0);

  gap_buffer<char> gb1(10, 'a');
  print_gb("gb1", gb1);

  std::string s("xyz");
  gap_buffer<char> gb2(s.begin(), s.end());
  print_gb("gb2", gb2);

  gap_buffer<char> gb3(gb2);
  print_gb("gb3", gb3);

  gap_buffer<char> gb4(std::move(gb1));
  print_gb("gb4", gb4);

  gap_buffer<char> gb5({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'});
  print_gb("gb5", gb5);

  print_gb("gb5", gb5 = gb0);
  print_gb("gb5", gb5 = std::move(gb2));

  gb4.swap(gb5);
  print_gb("gb4", gb4);
  print_gb("gb5", gb5);

  gap_buffer<char> gb6(10, 'a');
  print_gb("gb6", gb6);
  gb6.assign(8, 'b');
  print_gb("gb6", gb6);
  gb6.assign(s.begin(), s.end());
  print_gb("gb6", gb6);
  gb6.assign({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'});
  print_gb("gb6", gb6);

  std::cout << "gb6[1]='" << gb6[1] << "'" << std::endl;

  gb6.erase(gb6.begin() + 3, gb6.begin() + 5);
  print_gb("gb6", gb6);

  gb6.insert(gb6.begin(), s.begin(), s.end());
  print_gb("gb6", gb6);

  std::cout << "gb6.capacity()=" << gb6.capacity() << std::endl;
  gb6.reserve(100);
  print_gb("gb6", gb6);
  std::cout << "gb6.capacity()=" << gb6.capacity() << std::endl;

  std::cout << "gb6.size()=" << gb6.size() << std::endl;
  std::cout << "gb6.max_size()=" << gb6.max_size() << std::endl;

  std::cout << "gb6.at(3)=" << gb6.at(3) << std::endl;

  try {
    gb6.at(10);
  }
  catch (std::out_of_range& err) {
    std::cout << "gb6.at(10) out_of_range" << std::endl;
  }

  std::cout << "gb6.front()='" << gb6.front() << "'" << std::endl;
  std::cout << "gb6.back()='" << gb6.back() << "'" << std::endl;
  std::cout << "gb6.empty()=" << gb6.empty() << std::endl;

  gb6.shrink_to_fit();
  std::cout << "gb6.capacity()=" << gb6.capacity() << std::endl;

  gb6.clear();
  print_gb("gb6", gb6);

  gb6.resize(13, 'b');
  print_gb("gb6", gb6);

  gb6.insert(gb6.begin() + 5, 'c');
  print_gb("gb6", gb6);

  gb6.insert(gb6.begin() + 5, 3, 'o');
  print_gb("gb6", gb6);

  gb6.insert(gb6.begin() + 5, {'1', '2', '3'});
  print_gb("gb6", gb6);

  gb6.erase(gb6.begin() + 5);
  print_gb("gb6", gb6);

  gb6.emplace(gb6.begin() + 3, '1');
  print_gb("gb6", gb6);

  gb6.emplace_back('4');
  print_gb("gb6", gb6);

  gb6.push_back('5');
  print_gb("gb6", gb6);

  gb6.pop_back();
  print_gb("gb6", gb6);

  for (auto i = gb6.rbegin(); i != gb6.rend(); ++i)
    std::cout << *i;
  std::cout << std::endl;

  std::string s2 = "1234567890";
  gap_buffer<char> gb7(s2.begin(), s2.end());
  print_gb("gb7", gb7);
  gb7.replace(gb7.begin() + 1, gb7.begin() + 4, s2.rbegin() + 1, s2.rbegin() + 3);
  print_gb("gb7", gb7);

  auto gb8 = gb7.substr(gb7.begin() + 2, gb7.end());
  print_gb("gb8", gb8);
}