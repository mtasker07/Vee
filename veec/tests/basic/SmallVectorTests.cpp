/**
 * @file SmallVectorTests.cpp
 * @brief This file contains unit tests for the basic::SmallVector class.
 */

#include <gtest/gtest.h>

#include "veec/basic/SmallVector.hpp"

using veec::basic::SmallVector;

/**
 * TEST: UsesInlineStorage
 * 
 * Tests:
 * - Creating a SmallVector with 4 inline storage slots.
 * - Ensuring no heap allocation occurs when pushing exactly 4 elements.
 * 
 * Expected Result:
 * - The SmallVector should use inline storage for small sizes.
 * 
 * Notes:
 */
TEST(SmallVectorTests, UsesInlineStorage) {
    SmallVector<int, 4> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    EXPECT_TRUE(vec.is_inline());
}

/**
 * TEST: UsesHeapStorage
 * 
 * Tests:
 * - Creating a SmallVector with 4 inline storage slots.
 * - Ensuring heap allocation occurs when pushing more than 4 elements.
 * 
 * Expected Result:
 * - The SmallVector should use heap storage when exceeding inline capacity.
 * 
 * Notes:
 */
TEST(SmallVectorTests, UsesHeapStorage) {
    SmallVector<int, 4> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);

    EXPECT_FALSE(vec.is_inline());
}

/**
 * TEST: InitialisesWithCapacity
 * 
 * Tests:
 * - Creating a SmallVector with initial capacity.
 * - Ensuring the capacity matches the requested capacity.
 * - Ensuring heap allocation occurs if the initial capacity exceeds inline capacity.
 * 
 * Expected Result:
 * - The SmallVector should have the correct capacity and switch to heap storage.
 * 
 * Notes:
 */
TEST(SmallVectorTests, InitialisesWithCapacity) {
    SmallVector<int, 4> vec(10); // Initial capacity exceeds inline capacity

    EXPECT_EQ(vec.capacity(), 10);
    EXPECT_FALSE(vec.is_inline());
}

/**
 * TEST: InitialisesWithSizeAndValue
 * 
 * Tests:
 * - Creating a SmallVector with initial size and value.
 * - Ensuring the size matches the requested size.
 * - Ensuring all values are initialized correctly.
 * 
 * Expected Result:
 * - The SmallVector should have the correct size and all values should be initialized correctly.
 * 
 * Notes:
 */
TEST(SmallVectorTests, InitialisesWithSizeAndValue) {
    SmallVector<int, 4> vec(3, 42); // Initial size and value

    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 42);
    EXPECT_EQ(vec[1], 42);
    EXPECT_EQ(vec[2], 42);
    EXPECT_TRUE(vec.is_inline());
}
