//
// Created by robin on 09/04/2018.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "gap_buffer.h"

TEST_CASE("Gapbuffer are initialized", "[gapbuffer]") {

  SECTION("Initialize an empty gapbuffer.") {
    dr::gap_buffer<char> gb1(0);
    CHECK(gb1.empty());

    dr::gap_buffer<char> gb2{};
    CHECK(gb2.empty());
  }

  SECTION("Initialize a gapbuffer with content.") {
    dr::gap_buffer<char> gb1(3, 'a');
    CHECK(gb1.size() == 3);
    CHECK(gb1[0] == 'a');
    CHECK(gb1[1] == 'a');
    CHECK(gb1[2] == 'a');

    std::string s("xyzw");
    dr::gap_buffer<char> gb2(s.begin(), s.end());
    CHECK(gb2.size() == 4);
    CHECK(gb2[0] == 'x');
    CHECK(gb2[1] == 'y');
    CHECK(gb2[2] == 'z');
    CHECK(gb2[3] == 'w');

    dr::gap_buffer<char> gb3(gb2);
    CHECK(gb3.size() == 4);
    CHECK(gb3[0] == 'x');
    CHECK(gb3[1] == 'y');
    CHECK(gb3[2] == 'z');
    CHECK(gb3[3] == 'w');

    dr::gap_buffer<char> gb4(std::move(gb3));
    CHECK(gb4.size() == 4);
    CHECK(gb4[0] == 'x');
    CHECK(gb4[1] == 'y');
    CHECK(gb4[2] == 'z');
    CHECK(gb4[3] == 'w');

    dr::gap_buffer<char> gb5({'a', 'b', 'c', 'd'});
    CHECK(gb5.size() == 4);
    CHECK(gb5[0] == 'a');
    CHECK(gb5[1] == 'b');
    CHECK(gb5[2] == 'c');
    CHECK(gb5[3] == 'd');
  }

  SECTION("Assignments") {
    dr::gap_buffer<char> gb1{'a', 'b', 'c'};
    dr::gap_buffer<char> gb2;
    dr::gap_buffer<char> gb3(3, 'b');
    dr::gap_buffer<char> gb4;
    gb2 = gb1;
    CHECK(gb2.size() == 3);
    CHECK(gb2[0] == 'a');
    CHECK(gb2[1] == 'b');
    CHECK(gb2[2] == 'c');

    gb3 = std::move(gb1);
    CHECK(gb3.size() == 3);
    CHECK(gb3[0] == 'a');
    CHECK(gb3[1] == 'b');
    CHECK(gb3[2] == 'c');

    gb4.assign(3, 'a');
    CHECK(gb4.size() == 3);
    CHECK(gb4[0] == 'a');
    CHECK(gb4[1] == 'a');
    CHECK(gb4[2] == 'a');

    std::string s("abc");
    gb4.assign(s.begin(), s.end());
    CHECK(gb4.size() == 3);
    CHECK(gb4[0] == 'a');
    CHECK(gb4[1] == 'b');
    CHECK(gb4[2] == 'c');

    gb4.assign({'x', 'y', 'z'});
    CHECK(gb4.size() == 3);
    CHECK(gb4[0] == 'x');
    CHECK(gb4[1] == 'y');
    CHECK(gb4[2] == 'z');
  }

  SECTION("Swap") {
    dr::gap_buffer<char> gb1{'a', 'b', 'c'};
    dr::gap_buffer<char> gb2{'x', 'y', 'z'};
    gb1.swap(gb2);

    CHECK(gb1.size() == 3);
    CHECK(gb1[0] == 'x');
    CHECK(gb1[1] == 'y');
    CHECK(gb1[2] == 'z');
    CHECK(gb2.size() == 3);
    CHECK(gb2[0] == 'a');
    CHECK(gb2[1] == 'b');
    CHECK(gb2[2] == 'c');
  }

  SECTION("Insertion and deletion") {
    dr::gap_buffer<char> gb6({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'});
    
  }

}