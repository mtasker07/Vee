/**
 * @file ValueNameMap.hpp
 * @brief Contains the definition of the ValueNameMap class, which is
 * responsible for storing names of MIR values for debugging and visualization purposes.
 * We prefer an external map approach, as it avoids storing names in every node including
 * those that don't have names.
 */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/mir/MirFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {
namespace pretty {

/**
 * @class ValueNameMap
 * @brief Responsible for storing names of MIR values for debugging and visualization purposes.
 */
class ValueNameMap {
public:
    /**
     * @brief Creates a new ValueNameMap instance.
     */
    ValueNameMap() = default;
    ~ValueNameMap() = default;

    /**
     * @brief Checks if the given value has a name in the map.
     * @param value The value to check.
     * @return True if the value has a name, false otherwise.
     */
    inline bool isNamed(const Value* value) const {
        return _valueToName.find(value) != _valueToName.end();
    }
    /**
     * @brief Gets the name of the given value. If the value does not have a name,
     * returns an empty string view.
     * @param value The value for which to get the name.
     * @return The name of the value, or an empty string view if the value does not have a name.
     */
    inline std::string_view getName(const Value* value) const {
        auto it = _valueToName.find(value);
        if (it != _valueToName.end()) {
            return _names[it->second];
        }
        return {};
    }
    /**
     * @brief Sets the name of the given value. If the name is already taken, appends a suffix to make it
     * unique. E.g. "foo", "foo1", "foo2", etc.
     * @param value The value for which to set the name.
     * @param name The desired name for the value.
     */
    inline void setName(const Value* value, std::string_view name) {
        VEE_ASSERT(value != nullptr, "Value is null!");
        VEE_ASSERT(!name.empty(), "Name is empty!");

        u32& suffix = _nextSuffix[name];
        if (suffix == 0) {
            _valueToName[value] = _names.size();
            _names.push_back(std::string(name));
        } else {
            _valueToName[value] = _names.size();
            _names.push_back(std::string(name) + std::to_string(suffix));
        }
        ++suffix;
    }


private:
    std::unordered_map<std::string_view, u32> _nextSuffix;
    std::vector<std::string> _names;
    std::unordered_map<const Value*, size_t> _valueToName;
};

} // namespace pretty
} // namespace mir
VEEC_NAMESPACE_END
