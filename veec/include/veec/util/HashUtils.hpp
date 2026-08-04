/**
 * @file HashUtils.hpp
 * @brief This file contains hash utility functions.
 */

#pragma once

#include <functional>
#include <tuple>
#include <vector>

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

template<typename T>
inline void hashCombine(size_t& seed, const T& value);

template<typename T, typename Alloc>
inline void hashCombine(size_t& seed, const std::vector<T, Alloc>& values);
    
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
 * @brief Combines the hash of a vector of values into an existing seed. This seed
 * can then be used for further hashes.
 * @param seed The existing hash seed.
 * @param values The vector of values to hash and combine into the seed.
 */
template<typename T, typename Alloc>
inline void hashCombine(size_t& seed, const std::vector<T, Alloc>& values) {
    hashCombine(seed, values.size());
    for (const auto& value : values) {
        hashCombine(seed, value);
    }
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

/**
 * @struct CompositeKey
 * @brief A composite key that can be used in hash maps. It combines multiple values into a single key that
 * can be used along with CompositeHasher to create a hash map with multiple values as the key.
 * @tparam Ts The types of the values to combine.
 */
template<typename... Ts>
using CompositeKey = std::tuple<Ts...>;

/**
 * @struct CompositeHasher
 * @brief A hasher for CompositeKey.
 */
struct CompositeHasher {
    template<typename... Ts>
    std::size_t operator()(const CompositeKey<Ts...>& compositeKey) const {
        std::size_t seed = 0;
        std::apply([&](const auto&... values) {
            (hashCombine(seed, values), ...);
        }, compositeKey);
        return seed;
    }
};

} // namespace HashUtils

} // namespace util
VEEC_NAMESPACE_END
