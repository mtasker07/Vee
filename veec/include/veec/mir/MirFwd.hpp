/**
 * @file MIRFwd.hpp
 * @brief This file contains forward declarations for the MIR
 * nodes used in the Vee compiler.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

//
// ENUMS
//

//
// STRUCTS
//

//
// CLASSES
//

class MirContext;
class MirBuilder;

class Function;
class Block;
class Instruction;
class Value;
class Type;

} // namespace mir
VEEC_NAMESPACE_END
