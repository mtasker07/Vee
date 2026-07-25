cmake_minimum_required(VERSION 3.20)

include(UnitTesting)

vee_add_unit_tests(veec
    basic/APIntTests.cpp
    basic/BigIntTests.cpp
    lexing/LexerTests.cpp
)
