/**
 * @file BuiltinType.hpp
 * @brief This file contains the definition of the builtin type class.
 *
 * The builtin type class represents builtin types such as i32.
 */

#pragma once

#include <string>
#include <string_view>
#include <format>

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
 * @brief Converts a BuiltinTypeKind to its string representation.
 * @param kind The BuiltinTypeKind to convert.
 * @return A string representation of the BuiltinTypeKind.
 */
inline std::string_view toString(BuiltinTypeKind kind) {
    switch (kind) {
        case BuiltinTypeKind::Void: return "void";
        case BuiltinTypeKind::Bool: return "bool";
        case BuiltinTypeKind::String: return "string";
        case BuiltinTypeKind::I8: return "i8";
        case BuiltinTypeKind::I16: return "i16";
        case BuiltinTypeKind::I32: return "i32";
        case BuiltinTypeKind::I64: return "i64";
        case BuiltinTypeKind::U8: return "u8";
        case BuiltinTypeKind::U16: return "u16";
        case BuiltinTypeKind::U32: return "u32";
        case BuiltinTypeKind::U64: return "u64";
        case BuiltinTypeKind::F32: return "f32";
        case BuiltinTypeKind::F64: return "f64";
        default:
            VEE_UNREACHABLE("Unknown builtin type kind");
    }
}

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
     * @brief Converts this builtin type to a string representation.
     * @return A string representation of this builtin type.
     */
    std::string toString() const override {
        return std::format("{}", types::toString(_builtinKind));
    }

    /**
     * @brief Gets the kind of this builtin type.
     * @return The kind of this builtin type.
     */
    inline BuiltinTypeKind getBuiltinKind() const { return _builtinKind; }

    /**
     * @brief Checks if this builtin type is a void type.
     * @return True if this builtin type is a void type, false otherwise.
     */
    inline bool isVoid() const {
        return _builtinKind == BuiltinTypeKind::Void;
    }
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
     * @brief Checks if this builtin type is any signed integer type.
     * @return True if this builtin type is any signed integer type, false otherwise.
     */
    inline bool isSignedInteger() const {
        return _builtinKind == BuiltinTypeKind::I8 ||
               _builtinKind == BuiltinTypeKind::I16 ||
               _builtinKind == BuiltinTypeKind::I32 ||
               _builtinKind == BuiltinTypeKind::I64;
    }
    /**
     * @brief Checks if this builtin type is any unsigned integer type.
     * @return True if this builtin type is any unsigned integer type, false otherwise.
     */
    inline bool isUnsignedInteger() const {
        return _builtinKind == BuiltinTypeKind::U8 ||
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
     * @brief Checks if this builtin type is a string type.
     * @return True if this builtin type is a string type, false otherwise.
     */
    inline bool isString() const {
        return _builtinKind == BuiltinTypeKind::String;
    }

    /**
     * @brief Gets the bit width of this builtin type.
     * @return The bit width of this builtin type.
     * @note This function is only valid for integer and floating-point types.
     */
    inline u32 getBitWidth() const {
        VEE_ASSERT(isInteger() || isFloatingPoint(),
            "getBitWidth is only valid for integer and floating-point types");

        switch (_builtinKind) {
            case BuiltinTypeKind::I8:
            case BuiltinTypeKind::U8:
                return 8;
            case BuiltinTypeKind::I16:
            case BuiltinTypeKind::U16:
                return 16;
            case BuiltinTypeKind::I32:
            case BuiltinTypeKind::U32:
            case BuiltinTypeKind::F32:
                return 32;
            case BuiltinTypeKind::I64:
            case BuiltinTypeKind::U64:
            case BuiltinTypeKind::F64:
                return 64;
            default:
                VEE_UNREACHABLE("Builtin type does not have a bit width");
        }
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
