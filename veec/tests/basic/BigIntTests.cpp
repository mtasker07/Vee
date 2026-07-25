/**
 * @file BigIntTests.cpp
 * @brief This file contains unit tests for the basic::BigInt class.
 */

#include <gtest/gtest.h>

#include "veec/basic/BigInt.hpp"

using veec::basic::BigInt;

/**
 * TEST: FromU64
 * 
 * Tests:
 * - Creating a BigInt from a 64-bit unsigned integer.
 * - Converting the BigInt back to a string in base 10.
 * 
 * Expected Result:
 * - The string representation should match the original integer value.
 * 
 * Notes:
 */
TEST(BigIntTests, FromU64) {
    BigInt bigint = BigInt::fromU64(1234567890123456789ULL);
    EXPECT_EQ(bigint.toString(10), "1234567890123456789");
}

TEST(BigIntTests, FromStringDecimal) {
    BigInt bigint = BigInt::fromString("1234567890123456789", 10);
    EXPECT_EQ(bigint.toString(10), "1234567890123456789");
}

TEST(BigIntTests, FromStringHex) {
    BigInt bigint = BigInt::fromString("112210f47de98115", 16);
    EXPECT_EQ(bigint.toString(16), "112210f47de98115");
}

TEST(BigIntTests, FromStringBinary) {
    BigInt bigint = BigInt::fromString("100010010001000100001111010001111101111010011000000100010101", 2);
    EXPECT_EQ(bigint.toString(2), "100010010001000100001111010001111101111010011000000100010101");
}

TEST(BigIntTests, IgnoresLeadingWhitespaceAndZeros) {
    BigInt bigint = BigInt::fromString("   00042", 10);
    EXPECT_EQ(bigint.toString(10), "42");
}

TEST(BigIntTests, ZeroDoesNotKeepNegativeSign) {
    BigInt bigint = BigInt::fromString("0000", 10, true);
    EXPECT_TRUE(bigint.isZero());
    EXPECT_FALSE(bigint.isNegative());
    EXPECT_EQ(bigint.toString(10), "0");
}

TEST(BigIntTests, NegativeValueRoundTrips) {
    BigInt bigint = BigInt::fromString("42", 10, true);
    EXPECT_TRUE(bigint.isNegative());
    EXPECT_EQ(bigint.toString(10), "-42");
}

TEST(BigIntTests, AdditionWithPositiveValues) {
    BigInt lhs = BigInt::fromString("12345678901234567890", 10);
    BigInt rhs = BigInt::fromString("98765432109876543210", 10);
    EXPECT_EQ((lhs + rhs).toString(10), "111111111011111111100");
}

TEST(BigIntTests, AdditionWithDifferentSigns) {
    BigInt lhs = BigInt::fromU64(100);
    BigInt rhs = BigInt::fromU64(30, true);
    EXPECT_EQ((lhs + rhs).toString(10), "70");
}

TEST(BigIntTests, SubtractionCanProduceNegativeValue) {
    BigInt lhs = BigInt::fromU64(3);
    BigInt rhs = BigInt::fromU64(5);
    EXPECT_EQ((lhs - rhs).toString(10), "-2");
}

TEST(BigIntTests, MultiplicationCrosses64BitBoundary) {
    BigInt lhs = BigInt::fromString("4294967296", 10);
    BigInt rhs = BigInt::fromString("4294967296", 10);
    EXPECT_EQ((lhs * rhs).toString(10), "18446744073709551616");
}

TEST(BigIntTests, MultiplicationWithNegativeValue) {
    BigInt lhs = BigInt::fromU64(12, true);
    BigInt rhs = BigInt::fromU64(11);
    EXPECT_EQ((lhs * rhs).toString(10), "-132");
}

TEST(BigIntTests, DivisionReturnsExactQuotient) {
    BigInt lhs = BigInt::fromU64(1000);
    BigInt rhs = BigInt::fromU64(8);
    EXPECT_EQ((lhs / rhs).toString(10), "125");
}

TEST(BigIntTests, DivisionTruncatesTowardZero) {
    BigInt lhs = BigInt::fromU64(7, true);
    BigInt rhs = BigInt::fromU64(3);
    EXPECT_EQ((lhs / rhs).toString(10), "-2");
}

TEST(BigIntTests, BitwiseOperatorsWorkOnPositiveValues) {
    BigInt lhs = BigInt::fromString("aa55", 16);
    BigInt rhs = BigInt::fromString("0ff0", 16);

    EXPECT_EQ((lhs & rhs).toString(16), "a50");
    EXPECT_EQ((lhs | rhs).toString(16), "aff5");
    EXPECT_EQ((lhs ^ rhs).toString(16), "a5a5");
}

TEST(BigIntTests, PositiveValueReportsPositive) {
    BigInt bigint = BigInt::fromU64(42);
    EXPECT_TRUE(bigint.isPositive());
    EXPECT_FALSE(bigint.isNegative());
}

TEST(BigIntTests, LargeDecimalRoundTrips) {
    BigInt bigint = BigInt::fromString("1234567890123456789012345678901234567890", 10);
    EXPECT_EQ(bigint.toString(10), "1234567890123456789012345678901234567890");
}
