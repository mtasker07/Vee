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

VEEC_NAMESPACE_BEGIN
namespace types {

enum class ConversionRank : u8 {
    ExactMatch,
    Promotion,
    Conversion,
    UserDefinedConversion,
    NoConversion
};

struct ConversionRule {
    ConversionRank rank;
    Type* from;
    Type* to;
};

/**
 * @class TypeSystem
 * @brief Handles all type-related operations, such as conversion compatibility, etc.
 */
class TypeSystem {
public:
    bool canConvert(Type* from, Type* to) const;
    ConversionRank rankConversion(Type* from, Type* to) const;

private:
};

} // namespace types
VEEC_NAMESPACE_END
