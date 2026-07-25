/**
 * @file StringPool.hpp
 * @brief This file contains the definition of the StringPool class.
 * 
 * The StringPool is a data structure that stores unique strings and provides
 * an efficient lookup mechanism for retrieving them using a unique integer-based identifier
 * (StringId).
 * 
 * Whilst not only saving memory, this also allows for much faster string comparisons since
 * we're just comparing an integer instead of a string.
 */

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/StringId.hpp"

VEEC_NAMESPACE_BEGIN
namespace basic {

/**
 * @brief A StringPool is a data structure that stores unique strings
 * and provides a way to retrieve them using a unique identifier (StringId).
 */
class StringPool {
public:
    StringPool() = default;
    ~StringPool() = default;

    /**
     * @brief Interns a string into the pool and returns its unique identifier.
     * If the string already exists in the pool, its existing identifier is returned.
     * @param str The string to intern.
     * @return The unique identifier (StringId) for the interned string.
     */
    StringId intern(std::string_view str);
    /**
     * @brief Retrieves a string from the pool using its unique identifier.
     * @param id The unique identifier (StringId) of the string to retrieve.
     * @return The string corresponding to the given identifier.
     */
    std::string_view get(StringId id) const;

private:
    std::unordered_map<std::string, StringId> _stringToId;
    std::vector<std::string> _strings;
};

} // namespace basic
VEEC_NAMESPACE_END
