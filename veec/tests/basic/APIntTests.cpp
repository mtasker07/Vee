/**
 * @file APIntTests.cpp
 * @brief This file contains unit tests for the basic::APInt class.
 */

#include <gtest/gtest.h>

#include "veec/basic/APInt.hpp"

using veec::basic::APInt;

TEST(APIntTests, FromU64) {
    APInt apint = APInt::fromU64(64, 1234567890123456789ULL);
    EXPECT_EQ(apint.toString(10), "1234567890123456789");
}

TEST(APIntTests, FromStringDecimal) {
    APInt apint = APInt::fromString(64, "1234567890123456789", 10);
    EXPECT_EQ(apint.toString(10), "1234567890123456789");
}

TEST(APIntTests, FromStringHex) {
    APInt apint = APInt::fromString(64, "112210f47de98115", 16);
    EXPECT_EQ(apint.toString(16), "112210f47de98115");
}

TEST(APIntTests, FromStringBinary) {
    APInt apint = APInt::fromString(64, "100010010001000100001111010001111101111010011000000100010101", 2);
    EXPECT_EQ(apint.toString(2), "100010010001000100001111010001111101111010011000000100010101");
}

TEST(APIntTests, MaxValue) {
    APInt apint = APInt::max(64);
    EXPECT_EQ(apint.toString(10), "18446744073709551615");
}

TEST(APIntTests, ZeroValue) {
    APInt apint = APInt::fromU64(64, 0);
    EXPECT_TRUE(apint.isZero());
    EXPECT_FALSE(apint.isNegative());
    EXPECT_EQ(apint.toString(10), "0");
}

TEST(APIntTests, IgnoresLeadingWhitespaceAndZeros) {
    APInt apint = APInt::fromString(64, "   00042", 10);
    EXPECT_EQ(apint.toString(10), "42");
}

TEST(APIntTests, ConstructionMasksToBitWidth) {
    APInt apint = APInt::fromU64(8, 0x1FF);
    EXPECT_EQ(apint.toString(10), "255");
}

TEST(APIntTests, NegativeConstructionUsesTwosComplementStorage) {
    APInt apint = APInt::fromU64(8, 1, true);
    EXPECT_TRUE(apint.isNegative());
    EXPECT_EQ(apint.toString(10), "255");
}

TEST(APIntTests, AdditionWrapsToWidth) {
    APInt lhs = APInt::fromU64(8, 250);
    APInt rhs = APInt::fromU64(8, 10);
    EXPECT_EQ((lhs + rhs).toString(10), "4");
}

TEST(APIntTests, SubtractionWrapsToWidth) {
    APInt lhs = APInt::fromU64(8, 3);
    APInt rhs = APInt::fromU64(8, 5);
    EXPECT_EQ((lhs - rhs).toString(10), "254");
}

TEST(APIntTests, MultiplicationWrapsToWidth) {
    APInt lhs = APInt::fromU64(8, 20);
    APInt rhs = APInt::fromU64(8, 13);
    EXPECT_EQ((lhs * rhs).toString(10), "4");
}

TEST(APIntTests, DivisionReturnsExpectedQuotient) {
    APInt lhs = APInt::fromU64(16, 200);
    APInt rhs = APInt::fromU64(16, 10);
    EXPECT_EQ((lhs / rhs).toString(10), "20");
}

TEST(APIntTests, SignedDivisionReturnsRawTwosComplementBits) {
    APInt lhs = APInt::fromU64(8, 10, true);
    APInt rhs = APInt::fromU64(8, 2);
    APInt quotient = lhs / rhs;

    EXPECT_TRUE(quotient.isNegative());
    EXPECT_EQ(quotient.toString(10), "251");
}

TEST(APIntTests, BitwiseOperatorsWork) {
    APInt lhs = APInt::fromU64(8, 0xAA);
    APInt rhs = APInt::fromU64(8, 0xCC);

    EXPECT_EQ((lhs & rhs).toString(16), "88");
    EXPECT_EQ((lhs | rhs).toString(16), "ee");
    EXPECT_EQ((lhs ^ rhs).toString(16), "66");
}

TEST(APIntTests, ConvertsBetweenRadices) {
    APInt apint = APInt::fromString(16, "255", 10);
    EXPECT_EQ(apint.toString(16), "ff");
    EXPECT_EQ(apint.toString(2), "11111111");
}

TEST(APIntTests, ParseWrapsToWidth) {
    APInt apint = APInt::fromString(8, "511", 10);
    EXPECT_EQ(apint.toString(10), "255");
}

TEST(APIntTests, MaxPlusOneWrapsToZero) {
    APInt max = APInt::max(8);
    APInt one = APInt::fromU64(8, 1);
    APInt sum = max + one;

    EXPECT_TRUE(sum.isZero());
    EXPECT_EQ(sum.toString(10), "0");
}

TEST(APIntTests, PositiveValueReportsPositive) {
    APInt apint = APInt::fromU64(16, 42);
    EXPECT_TRUE(apint.isPositive());
    EXPECT_FALSE(apint.isNegative());
}

TEST(APIntTests, WidthZeroStaysZero) {
    APInt apint = APInt::fromU64(0, 123);
    EXPECT_EQ(apint.getBitWidth(), 0u);
    EXPECT_TRUE(apint.isZero());
    EXPECT_EQ(apint.toString(10), "0");
}
