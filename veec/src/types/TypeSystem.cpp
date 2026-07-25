#include "veec/types/TypeSystem.hpp"

#include <span>

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

bool TypeSystem::canConvert(Type* from, Type* to) const {
    return rankConversion(from, to) != ConversionRank::NoConversion;
}
ConversionRank TypeSystem::rankConversion(Type* from, Type* to) const {
    VEE_ASSERT(from != nullptr, "from type is null");
    VEE_ASSERT(to != nullptr, "to type is null");

    // Identical types -> exact match
    if (from == to) {
        return ConversionRank::ExactMatch;
    }

    // Lookup in conversion rules
    const ConversionRuleKey key{from, to};
    if (auto it = _conversionRules.find(key); it != _conversionRules.end()) {
        return it->second.rank;
    }

    return ConversionRank::NoConversion;
}

} // namespace types
VEEC_NAMESPACE_END
