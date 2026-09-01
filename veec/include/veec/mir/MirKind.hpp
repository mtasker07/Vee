/**
 * @file MirKind.hpp
 * @brief Contains the MirKind enum which is used to differentiate between different kinds of MIR nodes.
 * 
 * This is used for the exact same reason as AstKind, so for more information go read AstKind.hpp for
 * more info.
 * 
 * Since MIR node classes dont have a "Node" suffix, unlike AST kinds, the MIR kinds are just the name
 * of the class. Also, we do not bother with individual instruction kinds since they have their own
 * enum and dynamic system.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

/**
 * @brief Used to differentiate between different kinds of MIR nodes.
 */
enum class MirKind : u8 {
    Module,
    Function,
    Argument,
    BasicBlock,
    Instruction,
    Constant
};

} // namespace mir
VEEC_NAMESPACE_END
