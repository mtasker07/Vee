/**
 * @file BuiltinType.hpp
 * @brief This file contains the definition of the builtin type class.
 *
 * The builtin type class represents builtin types such as i32.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/types/TypeId.hpp"
#include "veec/types/TypeKind.hpp"
#include "veec/types/Type.hpp"

VEEC_NAMESPACE_BEGIN
namespace types {

/**
 * @enum BuiltinTypeKind
 * @brief Represents the different kinds of builtin types in the Vee language.
 */
enum class BuiltinTypeKind : u8 {
    Void,
    Bool,
    String,
    I8,
    I16,
    I32,
    I64,
    U8,
    U16,
    U32,
    U64,
    F32,
    F64,

    Count, // Number of builtin types
};

/**
 * @brief Represents a builtin Vee type.
 */
class BuiltinType : public Type {
public:
    /**
     * @brief Creates a builtin type with the specified builtin kind.
     * @param builtinKind The kind of the builtin type.
     */
    BuiltinType(BuiltinTypeKind builtinKind)
        : Type(TypeKind::Builtin), _builtinKind(builtinKind) {}

    virtual ~BuiltinType() = default;

    /**
     * @brief Gets the kind of this builtin type.
     * @return The kind of this builtin type.
     */
    inline BuiltinTypeKind getBuiltinKind() const { return _builtinKind; }

    /**
     * @brief Checks if this builtin type is a boolean type.
     * @return True if this builtin type is a boolean type, false otherwise.
     */
    inline bool isBoolean() const {
        return _builtinKind == BuiltinTypeKind::Bool;
    }
    /**
     * @brief Checks if this builtin type is any integer type (signed or unsigned).
     * @return True if this builtin type is any integer type, false otherwise.
     */
    inline bool isInteger() const {
        return _builtinKind == BuiltinTypeKind::I8 ||
               _builtinKind == BuiltinTypeKind::I16 ||
               _builtinKind == BuiltinTypeKind::I32 ||
               _builtinKind == BuiltinTypeKind::I64 ||
               _builtinKind == BuiltinTypeKind::U8 ||
               _builtinKind == BuiltinTypeKind::U16 ||
               _builtinKind == BuiltinTypeKind::U32 ||
               _builtinKind == BuiltinTypeKind::U64;
    }
    /**
     * @brief Checks if this builtin type is any floating-point type.
     * @return True if this builtin type is any floating-point type, false otherwise.
     */
    inline bool isFloatingPoint() const {
        return _builtinKind == BuiltinTypeKind::F32 ||
               _builtinKind == BuiltinTypeKind::F64;
    }

    /**
     * @brief Gets the static kind of this type, which is Builtin.
     * @return The static kind of this type, which is Builtin.
     */
    static TypeKind getStaticKind() { return TypeKind::Builtin; }

private:
    BuiltinTypeKind _builtinKind;
};

} // namespace types
VEEC_NAMESPACE_END
