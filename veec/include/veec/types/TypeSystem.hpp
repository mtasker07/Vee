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
 * @brief Represents the mode of a type conversion.
 */
enum class ConversionMode : u8 {
    /// @brief An automatic conversion that does not require a cast.
    Implicit,
    /// @brief A conversion that requires an explicit cast in the program.
    Explicit
};

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
    /**
     * @brief Constructs a new TypeSystem instance with the specified TypeTable.
     * @param typeTable The TypeTable instance to use for getting existing types.
     */
    explicit TypeSystem(TypeTable& typeTable)
        : _typeTable(typeTable) {}

    /**
     * @brief Adds a conversion rule to the type system.
     * @param from The source type of the conversion.
     * @param to The target type of the conversion.
     * @param rank The rank of the conversion.
     */
    void addConversionRule(Type* from, Type* to, ConversionRank rank);

    /**
     * @brief Checks if a conversion from one type to another is possible.
     * @param from The source type of the conversion.
     * @param to The target type of the conversion.
     * @param mode The mode of the conversion (implicit or explicit).
     * @return True if the conversion is possible, false otherwise.
     */
    bool canConvert(Type* from, Type* to, ConversionMode mode = ConversionMode::Implicit) const;
    /**
     * @brief Gets the rank of a conversion from one type to another.
     * @param from The source type of the conversion.
     * @param to The target type of the conversion.
     * @param mode The mode of the conversion (implicit or explicit).
     * @return The rank of the conversion, or ConversionRank::NoConversion if no conversion is possible.
     */
    ConversionRank rankConversion(Type* from, Type* to, ConversionMode mode = ConversionMode::Implicit) const;
    /**
     * @brief Gets the cost of a conversion from one type to another.
     * @param from The source type of the conversion.
     * @param to The target type of the conversion.
     * @param mode The mode of the conversion (implicit or explicit).
     * @return The cost of the conversion, or `std::numeric_limits<u32>::max()` if no
     * conversion is possible.
     */
    u32 conversionCost(Type* from, Type* to, ConversionMode mode = ConversionMode::Implicit) const;

private:
    struct ConversionRuleKey {
        Type* from;
        Type* to;

        bool operator==(const ConversionRuleKey& other) const {
            return from == other.from && to == other.to;
        }
    };

    struct ConversionRuleKeyHash {
        std::size_t operator()(const ConversionRuleKey& key) const {
            return std::hash<Type*>()(key.from) ^ std::hash<Type*>()(key.to);
        }
    };

    TypeTable& _typeTable;
    std::unordered_map<ConversionRuleKey, ConversionRule, ConversionRuleKeyHash> _conversionRules;
};

} // namespace types
VEEC_NAMESPACE_END
