/**
 * @file HashUtils.hpp
 * @brief This file contains hash utility functions.
 */

#pragma once

#include <functional>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace util {

/**
 * @namespace HashUtils
 * @brief Provides utility functions for hashing.
 */
namespace HashUtils {
    /**
     * @brief Hashes any given pointer.
     * @param ptr The pointer to hash.
     * @return The hash value of the pointer.
     */
    template<typename T>
    inline size_t hashPtr(const T* ptr) {
        return std::hash<const T*>{}(ptr);
    }
    /**
     * @brief Combines the hash of a value into an existing seed. This seed
     * can then be used for further hashes.
     * @param seed The existing hash seed.
     * @param value The value to hash and combine into the seed.
     */
    template<typename T>
    inline void hashCombine(size_t& seed, const T& value) {
        size_t h = std::hash<T>{}(value);
        seed ^= h + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
    }
    /**
     * @brief Hashes multiple values and combines them into a single hash value.
     * @param values The values to hash and combine.
     * @return The combined hash value of all the input values.
     */
    template<typename... Ts>
    size_t hashValues(const Ts&... values) {
        size_t seed = 0;
        (hashCombine(seed, values), ...);
        return seed;
    }
} // namespace HashUtils

} // namespace util
VEEC_NAMESPACE_END
