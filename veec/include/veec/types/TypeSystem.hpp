/**
 * @file TypeSystem.hpp
 * @brief This file contains the definition of the TypeSystem class.
 * 
 * The TypeSystem class handles all type-related operations, such as conversion compatibility.
 */

#pragma once

#include <span>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/types/TypeKind.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/types/TypeTable.hpp"

VEEC_NAMESPACE_BEGIN
namespace types {

/**
 * @brief Represents the rank of a type conversion.
 * The rank indicates the "quality" of the conversion, with lower ranks representing better conversions.
 */
enum class ConversionRank : u8 {
    /// @brief Types are the same.
    ExactMatch,
    /// @brief A safe widening conversion.
    Promotion,
    /// @brief A safe-ish conversion that may lose some information.
    Conversion,
    /// @brief An unsafe conversion that is potentially lossy or narrowing.
    NarrowingConversion,
    /// @brief A User-defined conversion.
    UserDefinedConversion,
    /// @brief An explicit conversion that requires a cast.
    ExplicitConversion,
    /// @brief No conversion is possible.
    NoConversion
};

/**
 * @brief Represents a type conversion rule.
 * A conversion rule defines how one type can be converted to another type,
 * along with the rank of the conversion.
 */
struct ConversionRule {
    /**
     * @brief The source type of the conversion.
     */
    Type* from;
    /**
     * @brief The target type of the conversion.
     */
    Type* to;
    /**
     * @brief The rank of the conversion.
     */
    ConversionRank rank;
};

/**
 * @class TypeSystem
 * @brief Handles all type-related operations, such as conversion compatibility, etc.
 */
class TypeSystem {
public:
    TypeSystem(const TypeTable& typeTable)
        : _typeTable(typeTable) {}

    std::span<const ConversionRule> getBuiltinConversionRules() const;

    bool canConvert(Type* from, Type* to) const;
    ConversionRank rankConversion(Type* from, Type* to) const;

private:
    const TypeTable& _typeTable;
};

} // namespace types
VEEC_NAMESPACE_END
