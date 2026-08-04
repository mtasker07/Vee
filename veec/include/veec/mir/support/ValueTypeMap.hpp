/**
 * @file ValueTypeMap.hpp
 * @brief Contains the definition of the ValueTypeMap class, which is
 * responsible for storing the types of all MIR values.
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
#include "veec/mir/Value.hpp"
#include "veec/mir/Instruction.hpp"
#include "veec/mir/support/InstructionTable.hpp"
#include "veec/types/TypeFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {
namespace support {

/**
 * @class ValueTypeMap
 * @brief Responsible for storing the types of all MIR values.
 */
class ValueTypeMap {
public:
    /**
     * @brief Creates a new ValueTypeMap instance.
     */
    ValueTypeMap() = default;
    ~ValueTypeMap() = default;

    /**
     * @brief Gets the type of a given MIR value.
     * @param value The MIR value whose type is to be retrieved.
     * @return The type of the given MIR value, or nullptr if not found.
     */
    inline types::Type* getValueType(const Value* value) const {
        VEE_ASSERT(value != nullptr, "Value cannot be null");

        auto it = _valueTypes.find(value);
        return it != _valueTypes.end() ? it->second : nullptr;
    }
    /**
     * @brief Sets the type of a given MIR value.
     * @param value The MIR value whose type is to be set.
     * @param type The type to be associated with the given MIR value.
     */
    inline void setValueType(const Value* value, types::Type* type) {
        VEE_ASSERT(value != nullptr, "Value cannot be null");
        VEE_ASSERT(type != nullptr, "Type cannot be null");
        _valueTypes[value] = type;
    }

private:
    std::unordered_map<const Value*, types::Type*> _valueTypes;
};

} // namespace support
} // namespace mir
VEEC_NAMESPACE_END
