/**
 * @file ArrayType.hpp
 * @brief This file contains the definition of the array type class.
 *
 * The array type class represents array types in the Vee language.
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
 * @brief Represents a array Vee type.
 */
class ArrayType : public Type {
public:
    /**
     * @brief Creates an array type with the specified element type and size.
     * @param elementType The type of the array elements.
     * @param size The size of the array.
     */
    ArrayType(Type* elementType, size_t size)
        : Type(TypeKind::Array), _elementType(elementType), _size(size) {}

    virtual ~ArrayType() = default;

    /**
     * @brief Gets the element type of this array type.
     * @return The element type of this array type.
     */
    inline Type* getElementType() const { return _elementType; }

    /**
     * @brief Gets the size of this array type.
     * @return The size of this array type.
     */
    inline size_t getSize() const { return _size; }

    /**
     * @brief Gets the static kind of this type, which is Array.
     * @return The static kind of this type, which is Array.
     */
    static TypeKind getStaticKind() { return TypeKind::Array; }

private:
    Type* _elementType;
    size_t _size;
};

} // namespace types
VEEC_NAMESPACE_END
