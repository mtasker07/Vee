/**
 * @file TypeFwd.hpp
 * @brief This file contains the forward declarations of all type classes.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace types {

//
// Enums
//

enum class TypeKind : u8;
enum class BuiltinTypeKind : u8;

//
// Types
//

class Type;
class ErrorType;
class BuiltinType;
class PointerType;
class ArrayType;
class FunctionType;
class ClassType;

} // namespace types
VEEC_NAMESPACE_END
