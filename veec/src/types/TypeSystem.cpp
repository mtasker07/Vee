#include "veec/types/TypeSystem.hpp"

#include <span>
#include <limits>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/types/Type.hpp"
#include "veec/types/ErrorType.hpp"
#include "veec/types/BuiltinType.hpp"
#include "veec/types/PointerType.hpp"
#include "veec/types/ArrayType.hpp"
#include "veec/types/FunctionType.hpp"
#include "veec/types/ClassType.hpp"

VEEC_NAMESPACE_BEGIN
namespace types {

void TypeSystem::addConversionRule(Type* from, Type* to, ConversionRank rank) {
    VEE_ASSERT(from != nullptr, "from type is null");
    VEE_ASSERT(to != nullptr, "to type is null");

    const ConversionRuleKey key{from, to};
    if (_conversionRules.find(key) != _conversionRules.end()) {
        VEE_FATAL("Conversion rule already exists for this type pair");
        return;
    }

    _conversionRules[key] = ConversionRule{from, to, rank};
}

bool TypeSystem::canConvert(Type* from, Type* to, ConversionMode mode) const {
    return rankConversion(from, to, mode) != ConversionRank::NoConversion;
}
ConversionRank TypeSystem::rankConversion(Type* from, Type* to, ConversionMode mode) const {
    VEE_ASSERT(from != nullptr, "from type is null");
    VEE_ASSERT(to != nullptr, "to type is null");

    // Identical types -> exact match
    if (from == to) {
        return ConversionRank::ExactMatch;
    }

    // Lookup in conversion rules
    auto it = _conversionRules.find({from, to}); 
    if (it == _conversionRules.end()) {
        return ConversionRank::NoConversion;
    }

    ConversionRank rank = it->second.rank;

    // Ignore explicits if implicit mode
    if (mode == ConversionMode::Implicit && rank == ConversionRank::ExplicitConversion) {
        return ConversionRank::NoConversion;
    }

    return rank;
}
ConversionCost TypeSystem::conversionCost(Type* from, Type* to, ConversionMode mode) const {
    ConversionRank rank = rankConversion(from, to, mode);

    switch (rank) {
        case ConversionRank::ExactMatch:
            return 0;
        case ConversionRank::Promotion:
            return 1;
        case ConversionRank::Conversion:
            return 2;
        case ConversionRank::NarrowingConversion:
            return 3;
        case ConversionRank::UserDefinedConversion:
            return 4;
        case ConversionRank::ExplicitConversion:
            if (mode == ConversionMode::Explicit) {
                return 5;
            } else {
                return std::numeric_limits<ConversionCost>::max();
            }
        case ConversionRank::NoConversion:
            return std::numeric_limits<ConversionCost>::max();

        default:
            VEE_UNREACHABLE("Unknown conversion rank");
    }

} // namespace types
VEEC_NAMESPACE_END
