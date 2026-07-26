/**
 * @file ErrorType.hpp
 * @brief This file contains the definition of the ErrorType class.
 *
 * The error type class represents the error type in the Vee language.
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
 * @brief Represents the error type in the Vee language.
 */
class ErrorType : public Type {
public:
    /**
     * @brief Creates an error type.
     */
    ErrorType()
        : Type(TypeKind::Error) {}

    virtual ~ErrorType() = default;

    /**
     * @brief Converts this error type to a string representation.
     * @return A string representation of this error type.
     */
    std::string toString() const override {
        return "<Error>";
    }

    /**
     * @brief Gets the static kind of this type, which is Error.
     * @return The static kind of this type, which is Error.
     */
    static TypeKind getStaticKind() { return TypeKind::Error; }
};

} // namespace types
VEEC_NAMESPACE_END
