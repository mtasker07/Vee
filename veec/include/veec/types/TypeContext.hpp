/**
 * @file TypeContext.hpp
 * @brief This file contains the definition of the TypeContext class,
 * which is used to hold context for type-related objects.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/types/TypeTable.hpp"
#include "veec/types/TypeSystem.hpp"

VEEC_NAMESPACE_BEGIN
namespace types {

/**
 * @class TypeContext
 * @brief Used to hold context for type-related objects.
 */
class TypeContext {
public:
    types::TypeTable table;
    types::TypeSystem system;

    /**
     * @brief Creates a new TypeContext instance.
     */
    TypeContext()
        : system(table) {}

    ~TypeContext() = default;
};

} // namespace types
VEEC_NAMESPACE_END
