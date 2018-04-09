//
// Created by robin on 09/04/2018.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "gap_buffer.h"
#include <string>

TEST_CASE("Gapbuffer are initialized", "[gapbuffer]") {

  SECTION("Initialize an empty gapbuffer.") {
    dr::gap_buffer<char> gb1(0);
    CHECK(gb1.empty());

    dr::gap_buffer<char> gb2{};
    CHECK(gb2.empty());
  }

  SECTION("Initialize a gapbuffer with content.") {
    dr::gap_buffer<char> gb(3,'a');
    CHECK(gb.size() == 3);
    CHECK(gb[0] == 'a');
    CHECK(gb[1] == 'a');
    CHECK(gb[2] == 'a');

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
}