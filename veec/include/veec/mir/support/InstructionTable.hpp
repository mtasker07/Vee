/**
 * @file InstructionTable.hpp
 * @brief This file contains the definition of the InstructionTable class.
 * 
 * The InstructionTable class represents a table of instruction overloads in the MIR.
 */

#pragma once

#include <vector>
#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/SmallVector.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/Instruction.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/types/TypeFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {
namespace support {

struct InstructionOverload {
    basic::SmallVector<types::Type*, 2> operandTypes;
    types::Type* resultType = nullptr;
};

/**
 * @class InstructionTable
 * @brief Represents a table of instruction overloads in the MIR.
 */
class InstructionTable {
public:
    InstructionTable() = default;
    ~InstructionTable() = default;

    inline const std::vector<InstructionOverload>& getOverloads(InstructionOpcode opcode) const {
        static const std::vector<InstructionOverload> empty;
        auto it = _overloads.find(opcode);
        return it != _overloads.end() ? it->second : empty;
    }
    /**
     * @brief Adds an overload for a given instruction opcode.
     * @param opcode The instruction opcode.
     * @param operandTypes The types of the operands for this overload.
     */
    void addOverload(
        InstructionOpcode opcode,
        const std::vector<types::Type*>& operandTypes,
        types::Type* resultType
    ) {
        VEE_ASSERT(resultType != nullptr, "Result type cannot be null");
        if (_resultTypes.find(opcode) != _resultTypes.end()) {
            VEE_ASSERT(_resultTypes[opcode] == resultType,
                "All overloads for a given opcode must have the same result type");
        }

        _resultTypes[opcode] = resultType;
        _overloads[opcode].push_back({operandTypes, resultType});
    }

    inline types::Type* getResultType(InstructionOpcode opcode) const {
        auto it = _resultTypes.find(opcode);
        if (it != _resultTypes.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
    std::unordered_map<InstructionOpcode, types::Type*> _resultTypes;
    std::unordered_map<InstructionOpcode, std::vector<InstructionOverload>> _overloads;
};

} // namespace support
} // namespace mir
VEEC_NAMESPACE_END
