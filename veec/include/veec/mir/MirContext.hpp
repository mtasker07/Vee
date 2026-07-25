/**
 * @file MirContext.hpp
 * @brief This file contains the definition of the MirContext class.
 * 
 * The MirContext class represents the context for the middle intermediate representation (MIR) of the compiler.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Arena.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

/**
 * @class MirContext
 * @brief Represents the context for the middle intermediate representation (MIR) of the compiler.
 */
class MirContext {
public:
    /**
     * @brief Creates a new MIRContext instance.
     */
    MirContext() = default;
    ~MirContext() = default;

private:
    // Allow all MIR classes to alloc
    friend class MirBuilder;
    friend class Function;
    friend class Block;
    friend class Instruction;
    friend class Value;
    friend class Type;

    basic::Arena<> _arena;
};

} // namespace mir
VEEC_NAMESPACE_END
