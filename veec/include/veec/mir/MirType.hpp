/**
 * @file MirType.hpp
 * @brief This file contains the definition of the MirType class, related types and all its sub-type classes.
 * 
 * The MirType class represents a type in the MIR.
 */

#pragma once

#include <vector>
#include <string>
#include <format>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/SmallVector.hpp"
#include "veec/mir/MirFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

// Forward types
class MirType;
class MirVoidType;
class MirBooleanType;
class MirIntegerType;
class MirFloatType;
class MirPointerType;
class MirArrayType;
class MirStructType;
class MirFunctionType;

enum class MirTypeKind : u8 {
    Void,
    Bool,
    Integer,
    Float,    
    Pointer,
    Array,
    Struct,
    Function
};

/**
 * @class MirType
 * @brief Represents a type in the MIR.
 */
class MirType {
public:
    virtual ~MirType() = default;

    /**
     * @brief Gets the kind of this type.
     * @return The kind of this type.
     */
    MirTypeKind getKind() const { return _kind; }

    /**
     * @brief Checks if this type is of the specified kind.
     * @param kind The kind to check against.
     * @return True if this type is of the specified kind, false otherwise.
     */
    inline bool isOfKind(MirTypeKind kind) const { return _kind == kind; }
    /**
     * @brief Checks if this type is a void type.
     * @return True if this type is a void type, false otherwise.
     */
    inline bool isVoid() const { return isOfKind(MirTypeKind::Void); }
    /**
     * @brief Checks if this type is a bool type.
     * @return True if this type is a bool type, false otherwise.
     */
    inline bool isBool() const { return isOfKind(MirTypeKind::Bool); }
    /**
     * @brief Checks if this type is an integer type.
     * @return True if this type is an integer type, false otherwise.
     */
    inline bool isInteger() const { return isOfKind(MirTypeKind::Integer); }
    /**
     * @brief Checks if this type is a float type.
     * @return True if this type is a float type, false otherwise.
     */
    inline bool isFloat() const { return isOfKind(MirTypeKind::Float); }
    /**
     * @brief Checks if this type is a pointer type.
     * @return True if this type is a pointer type, false otherwise.
     */
    inline bool isPointer() const { return isOfKind(MirTypeKind::Pointer); }
    /**
     * @brief Checks if this type is an array type.
     * @return True if this type is an array type, false otherwise.
     */
    inline bool isArray() const { return isOfKind(MirTypeKind::Array); }
    /**
     * @brief Checks if this type is a struct type.
     * @return True if this type is a struct type, false otherwise.
     */
    inline bool isStruct() const { return isOfKind(MirTypeKind::Struct); }
    /**
     * @brief Checks if this type is a function type.
     * @return True if this type is a function type, false otherwise.
     */
    inline bool isFunction() const { return isOfKind(MirTypeKind::Function); }
    
    /**
     * @brief Converts this type to a void type if this type is a void type (read-only).
     * @return A pointer to the MirVoidType instance if this type is a void type, nullptr otherwise.
     */
    const MirVoidType* asVoid() const;
    /**
     * @brief Converts this type to a bool type if this type is a bool type (read-only).
     * @return A pointer to the MirBoolType instance if this type is a bool type, nullptr otherwise.
     */
    const MirBoolType* asBool() const;
    /**
     * @brief Converts this type to an integer type if this type is an integer type (read-only).
     * @return A pointer to the MirIntegerType instance if this type is an integer type, nullptr otherwise.
     */
    const MirIntegerType* asInteger() const;
    /**
     * @brief Converts this type to a float type if this type is a float type (read-only).
     * @return A pointer to the MirFloatType instance if this type is a float type, nullptr otherwise.
     */
    const MirFloatType* asFloat() const;
    /**
     * @brief Converts this type to a pointer type if this type is a pointer type (read-only).
     * @return A pointer to the MirPointerType instance if this type is a pointer type, nullptr otherwise.
     */
    const MirPointerType* asPointer() const;
    /**
     * @brief Converts this type to an array type if this type is an array type (read-only).
     * @return A pointer to the MirArrayType instance if this type is an array type, nullptr otherwise.
     */
    const MirArrayType* asArray() const;
    /**
     * @brief Converts this type to a struct type if this type is a struct type (read-only).
     * @return A pointer to the MirStructType instance if this type is a struct type, nullptr otherwise.
     */
    const MirStructType* asStruct() const;
    /**
     * @brief Converts this type to a function type if this type is a function type (read-only).
     * @return A pointer to the MirFunctionType instance if this type is a function type, nullptr otherwise.
     */
    const MirFunctionType* asFunction() const;

    /**
     * @brief Converts this type to a void type if this type is a void type.
     * @return A pointer to the MirVoidType instance if this type is a void type, nullptr otherwise.
     */
    MirVoidType* asVoid();
    /**
     * @brief Converts this type to a bool type if this type is a bool type.
     * @return A pointer to the MirBoolType instance if this type is a bool type, nullptr otherwise.
     */
    MirBoolType* asBool();
    /**
     * @brief Converts this type to an integer type if this type is an integer type.
     * @return A pointer to the MirIntegerType instance if this type is an integer type, nullptr otherwise.
     */
    MirIntegerType* asInteger();
    /**
     * @brief Converts this type to a float type if this type is a float type.
     * @return A pointer to the MirFloatType instance if this type is a float type, nullptr otherwise.
     */
    MirFloatType* asFloat();
    /**
     * @brief Converts this type to a pointer type if this type is a pointer type.
     * @return A pointer to the MirPointerType instance if this type is a pointer type, nullptr otherwise.
     */
    MirPointerType* asPointer();
    /**
     * @brief Converts this type to an array type if this type is an array type.
     * @return A pointer to the MirArrayType instance if this type is an array type, nullptr otherwise.
     */
    MirArrayType* asArray();
    /**
     * @brief Converts this type to a struct type if this type is a struct type.
     * @return A pointer to the MirStructType instance if this type is a struct type, nullptr otherwise.
     */
    MirStructType* asStruct();
    /**
     * @brief Converts this type to a function type if this type is a function type.
     * @return A pointer to the MirFunctionType instance if this type is a function type, nullptr otherwise.
     */
    MirFunctionType* asFunction();

    //
    // Type utilities/helpers
    //

    /**
     * @brief Gets the bit width of this type if its an integer or float type.
     * @return The bit width of this type if it is an integer or float type, 0 otherwise.
     */
    u32 getBitWidth() const;

    /**
     * @brief Checks if the type is signed. Only valid on integer types.
     * @return True if the type is signed, false otherwise.
     */
    bool isSigned() const;

    /**
     * @brief Converts this type to a string representation.
     * @return A string representing this type.
     */
    virtual std::string toString() const = 0;

protected:
    /**
     * @brief Creates a new MirType instance with the specified kind.
     * @param kind The kind of the type.
     */
    MirType(MirTypeKind kind)
        : _kind(kind) {}

private:
    MirTypeKind _kind;
};

/**
 * @class MirVoidType
 * @brief Represents a void type.
 */
class MirVoidType final : public MirType {
public:
    /**
     * @brief Creates a new MirVoidType instance.
     */
    MirVoidType()
        : MirType(MirTypeKind::Void) {}

    /**
     * @brief Converts this void type to a string representation.
     * @return A string representing this void type ("void").
     */
    std::string toString() const override {
        return "void";
    }
};

/**
 * @class MirBoolType
 * @brief Represents a boolean type.
 */
class MirBoolType final : public MirType {
public:
    /**
     * @brief Creates a new MirBoolType instance.
     */
    MirBoolType()
        : MirType(MirTypeKind::Bool) {}

    /**
     * @brief Converts this boolean type to a string representation.
     * @return A string representing this boolean type ("bool").
     */
    std::string toString() const override {
        return "bool";
    }
};

/**
 * @class MirIntegerType
 * @brief Represents an integer type.
 */
class MirIntegerType : public MirType {
public:
    /**
     * @brief Creates a new MirIntegerType instance with the specified bit width.
     * All MIR integers are signless by default.
     * @param bitWidth The bit width of the integer type (default is 32).
     */
    MirIntegerType(u32 bitWidth = 32u, bool isSigned = false)
        : MirType(MirTypeKind::Integer), _bitWidth(bitWidth), _isSigned(isSigned)
    {
        validateBitWidth(bitWidth);
    }

    /**
     * @brief Gets the bit width of the integer type.
     * @return The bit width of the integer type.
     */
    u32 getBitWidth() const { return _bitWidth; }
    /**
     * @brief Checks if the integer type is signed.
     * @return True if the integer type is signed, false otherwise.
     */
    bool isSigned() const { return _isSigned; }

    /**
     * @brief Converts this integer type to a string representation.
     * @return A string representing this integer type (e.g., "i32", "i64", "u16").
     */
    std::string toString() const override {
        return std::format("{}{}", _isSigned ? "i" : "u", _bitWidth);
    }

private:
    u32 _bitWidth = 32u;
    bool _isSigned = false;

    static void validateBitWidth(u32 bitWidth) {
        VEE_ASSERT(bitWidth > 0, "Integer bit width must be greater than 0");
        VEE_ASSERT(bitWidth <= 64, "Integer bit width must be less than or equal to 64");
    }
};

/**
 * @class MirFloatType
 * @brief Represents a floating-point type.
 */
class MirFloatType : public MirType {
public:
    /**
     * @brief Creates a new MirFloatType instance with the specified bit width.
     * @param bitWidth The bit width of the float type (default is 32).
     */
    MirFloatType(u32 bitWidth = 32u)
        : MirType(MirTypeKind::Float), _bitWidth(bitWidth)
    {
        validateBitWidth(bitWidth);
    }

    /**
     * @brief Gets the bit width of the float type.
     * @return The bit width of the float type.
     */
    u32 getBitWidth() const { return _bitWidth; }

    /**
     * @brief Converts this float type to a string representation.
     * @return A string representing this float type (e.g., "f32", "f64").
     */
    std::string toString() const override {
        return std::format("f{}", _bitWidth);
    }

private:
    u32 _bitWidth = 32u;

    static void validateBitWidth(u32 bitWidth) {
        // Might change this later
        VEE_ASSERT(bitWidth == 32 || bitWidth == 64, "Float bit width must be either 32 or 64");
    }
};

/**
 * @class MirPointerType
 * @brief Represents a pointer type.
 */
class MirPointerType : public MirType {
public:
    /**
     * @brief Creates a new MirPointerType instance with the specified pointee type.
     * @param pointeeType The type of the element that this pointer points to.
     */
    MirPointerType(const MirType* pointeeType)
        : MirType(MirTypeKind::Pointer), _pointeeType(pointeeType) {}

    /**
     * @brief Gets the type of the element that this pointer points to (read-only).
     * @return The type of the element that this pointer points to.
     */
    inline const MirType* getPointeeType() const { return _pointeeType; }

    /**
     * @brief Converts this pointer type to a string representation.
     * @return A string representing this pointer type (e.g., "i32*", "f64*").
     */
    std::string toString() const override {
        return std::format("{}*", _pointeeType->toString());
    }

private:
    const MirType* _pointeeType = nullptr;
};

/**
 * @class MirArrayType
 * @brief Represents an array type.
 */
class MirArrayType : public MirType {
public:
    /**
     * @brief Creates a new MirArrayType instance with the specified element type and size.
     * @param elementType The type of the elements in the array.
     * @param size The number of elements in the array.
     */
    MirArrayType(const MirType* elementType, u64 size)
        : MirType(MirTypeKind::Array), _elementType(elementType), _size(size) {}

    /**
     * @brief Gets the element type of the array (read-only).
     * @return The element type of the array.
     */
    const MirType* getElementType() const { return _elementType; }
    /**
     * @brief Gets the number of elements in the array.
     * @return The number of elements in the array.
     */
    u64 getSize() const { return _size; }

    /**
     * @brief Converts this array type to a string representation.
     * @return A string representing this array type (e.g., "[4 x i32]").
     */
    std::string toString() const override {
        return std::format("[{} x {}]", _size, _elementType->toString());
    }

private:
    const MirType* _elementType = nullptr;
    u64 _size = 0u;
};

/**
 * @class MirStructType
 * @brief Represents a struct type.
 */
class MirStructType : public MirType {
public:
    /**
     * @brief Creates a new MirStructType instance.
     * This struct is considered opaque until setMemberTypes() or addMemberType() is called.
     * @param structId The unique identifier for the struct.
     * @param name The name of the struct (optional).
     */
    MirStructType(
        u32 structId,
        std::string_view name = {}
    )
        : MirType(MirTypeKind::Struct),
        _id(structId),
        _name(name) {}
    /**
     * @brief Creates a new MirStructType instance with the specified member types.
     * @param structId The unique identifier for the struct.
     * @param memberTypes The types of the members in the struct.
     * @param name The name of the struct (optional).
     */
    MirStructType(
        u32 structId,
        const basic::SmallVector<const MirType*>& memberTypes,
        std::string_view name = {}
    )
        : MirType(MirTypeKind::Struct),
        _id(structId),
        _memberTypes(memberTypes),
        _name(name) {}

    /**
     * @brief Gets the unique identifier of the struct.
     * @return The unique identifier of the struct.
     */
    inline u32 getId() const {
        return _id;
    }

    /**
     * @brief Gets the types of the members in the MirStructType (read-only).
     * @return The types of the members in the MirStructType.
     */
    inline const basic::SmallVector<const MirType*>& getMemberTypes() const {
        return _memberTypes;
    }
    /**
     * @brief Sets the types of the members in the MirStructType.
     * @param memberTypes The types of the members to set.
     */
    inline void setMemberTypes(const basic::SmallVector<const MirType*>& memberTypes) {
        _memberTypes = memberTypes;
    }
    /**
     * @brief Adds a member type to the MirStructType.
     * @param memberType The type of the member to add.
     */
    inline void addMemberType(const MirType* memberType) {
        _memberTypes.push_back(memberType);
    }

    /**
     * @brief Gets the name of the struct.
     * @return The name of the struct.
     */
    inline std::string_view getName() const {
        return _name;
    }
    /**
     * @brief Sets the name of the struct.
     * @param name The name of the struct to set.
     */
    inline void setName(std::string_view name) {
        _name = name;
    }

    /**
     * @brief Converts this struct type to a string representation.
     * @return A string representation of the struct type.
     */
    inline std::string toString() const {
        if (!_name.empty()) {
            return std::format("struct {}", _name);
        }
        return std::format("struct #{}", _id);
    }

private:
    u32 _id = 0;
    basic::SmallVector<const MirType*> _memberTypes = {};
    std::string_view _name = {};
};

/**
 * @class MirFunctionType
 * @brief Represents a function type.
 */
class MirFunctionType : public MirType {
public:
    /**
     * @brief Creates a new MirFunctionType instance with the specified return type and parameter types.
     * @param returnType The return type of the function.
     * @param parameterTypes The types of the parameters of the function.
     */
    MirFunctionType(const MirType* returnType, const basic::SmallVector<const MirType*>& parameterTypes)
        : MirType(MirTypeKind::Function), _returnType(returnType), _parameterTypes(parameterTypes) {}

    /**
     * @brief Gets the return type of the function (read-only).
     * @return The return type of the function.
     */
    inline const MirType* getReturnType() const { return _returnType; }
    /**
     * @brief Gets the types of the parameters of the function (read-only).
     * @return The types of the parameters of the function.
     */
    inline const basic::SmallVector<const MirType*>& getParameterTypes() const { return _parameterTypes; }

    /**
     * @brief Converts this function type to a string representation.
     * @return A string representation of the function type (e.g.
     * "func(i32, f64) -> void").
     */
    inline std::string toString() const {
        std::string result = "func(";
        for (size_t i = 0; i < _parameterTypes.size(); ++i) {
            result += _parameterTypes[i]->toString();
            if (i < _parameterTypes.size() - 1) {
                result += ", ";
            }
        }
        result += ") -> " + _returnType->toString();
        return result;
    }

private:
    const MirType* _returnType = nullptr;
    basic::SmallVector<const MirType*> _parameterTypes;
};

} // namespace mir
VEEC_NAMESPACE_END