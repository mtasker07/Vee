/**
 * @file MirContext.hpp
 * @brief This file contains the definition of the MirContext class.
 * 
 * The MirContext class represents the context for the middle intermediate representation (MIR) of the compiler.
 */

#pragma once

#include <type_traits>
#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Arena.hpp"
#include "veec/mir/MirFactory.hpp"
#include "veec/mir/support/InstructionTable.hpp"
#include "veec/mir/support/ValueTypeMap.hpp"
#include "veec/mir/pretty/ValueNameMap.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

/**
 * @class MirContext
 * @brief Represents the context for the middle intermediate representation (MIR) of the compiler.
 */
class MirContext {
public:
    support::InstructionTable instructionTable;
    support::ValueTypeMap valueTypes;
    pretty::ValueNameMap valueNames;
    MirFactory factory;

    /**
     * @brief Creates a new MIRContext instance.
     */
    MirContext()
        : factory(_nodeArena, valueTypes, valueNames) {}

    ~MirContext() = default;

    // Disallow copying and moving
    MirContext(const MirContext&) = delete;
    MirContext& operator=(const MirContext&) = delete;
    MirContext(MirContext&&) = delete;
    MirContext& operator=(MirContext&&) = delete;

private:
    basic::Arena<> _nodeArena;
};

} // namespace mir
VEEC_NAMESPACE_END
