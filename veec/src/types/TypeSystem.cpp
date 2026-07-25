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

    // TODO

    return ConversionRank::NoConversion;
}

} // namespace types
VEEC_NAMESPACE_END
