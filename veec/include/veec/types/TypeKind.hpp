/**
 * @file TypeKind.hpp
 * @brief This file contains the definition of the TypeKind enum.
 *
 * The type kind enum represents the different kinds of types in the Vee language.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace types {

/**
 * @enum TypeKind
 * @brief Represents the kind of a type.
 */
enum class TypeKind : u8 {
    Error,
    Builtin,
    Pointer,
    Array,
    Function,
    Class,
};

} // namespace types
VEEC_NAMESPACE_END
