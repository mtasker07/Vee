# Tests

This directory contains the unit tests for the Vee compiler.

## Overview
The vee compiler uses GoogleTest for unit testing. Each class in the project should have a corresponding unit test file. For example, lexing/Lexer.cpp has a corresponding tests/lexing/LexerTests.cpp file.

## Adding tests
Adding tests is relatively straight forward. First open or create the tests file for the class or module you want to test, then copy and paste the following code:

```cpp
/**
 * TEST: <TestName>
 * 
 * Tests:
 * <What it tests>
 * 
 * Expected Result:
 * 
 * Notes: (OPTIONAL)
 */
TEST(<...>Tests, <TestName>) {
    // Logic goes here
}
```

For example, a good test looks something like this:
```cpp
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
TEST(...Tests, TestName) {
    BigInt bigint = BigInt::fromU64(1234567890123456789ULL);
    EXPECT_EQ(bigint.toString(10), "1234567890123456789");
}
```
