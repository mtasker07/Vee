/**
 * @file PointerType.hpp
 * @brief This file contains the definition of the pointer type class.
 *
 * The pointer type class represents pointer types in the Vee language.
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
 * @brief Represents a pointer Vee type.
 */
class PointerType : public Type {
public:
    /**
     * @brief Creates a pointer type with the specified pointee type.
     * @param pointeeType The type that this pointer points to.
     */
    PointerType(Type* pointeeType)
        : Type(TypeKind::Pointer), _pointeeType(pointeeType) {}

    virtual ~PointerType() = default;

    /**
     * @brief Converts this pointer type to a string representation.
     * @return A string representation of this pointer type.
     */
    std::string toString() const override {
        return std::format("{}*", _pointeeType->toString());
    }

    /**
     * @brief Gets the type that this pointer points to.
     * @return The type that this pointer points to.
     */
    inline Type* getPointeeType() const { return _pointeeType; }

    /**
     * @brief Gets the static kind of this type, which is Pointer.
     * @return The static kind of this type, which is Pointer.
     */
    static TypeKind getStaticKind() { return TypeKind::Pointer; }

private:
    Type* _pointeeType;
};

} // namespace types
VEEC_NAMESPACE_END
