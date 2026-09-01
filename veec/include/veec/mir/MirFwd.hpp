/**
 * @file MirFwd.hpp
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

enum class MirKind : u8;
enum class InstructionOpcode : u8;
enum class ConstantKind : u8;
enum class MirTypeKind : u8;

//
// STRUCTS
//

//
// CLASSES
//

class MirContext;
class MirBuilder;
class MirFactory;

//
// CLASSES (MIR NODES)
//

class MirNode;
class Module;
class Function;
class BasicBlock;
class Instruction;
class Value;
class User;
class Argument;
class Constant;
class ConstantInt;
class ConstantFloat;
class ConstantString;
class ConstantBool;

//
// CLASSES (TYPES)
//

class MirType;
class MirVoidType;
class MirBoolType;
class MirIntegerType;
class MirFloatType;
class MirPointerType;
class MirArrayType;
class MirFunctionType;
class MirStructType;

} // namespace mir
VEEC_NAMESPACE_END
