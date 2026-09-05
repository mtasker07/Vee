#include "veec/codegen/support/NameMangler.hpp"

#include <string>
#include <string_view>
#include <sstream>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/Function.hpp"
#include "veec/mir/MirType.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace support {

std::string NameMangler::mangleFunction(const mir::Function&) {
    /*
    std::ostringstream result;
    result << "_VF_";
    result << func.getName();
    result << "_";
    // TODO: Handle generics
    for (const auto& param : sym->getParameters()) {
        result << mangleType(param->getType());
    }
    return result.str();
    */

    // TODO: Implement proper mangling
    static u32 fnCounter = 0;
    return "func" + std::to_string(fnCounter++);
}
std::string NameMangler::mangleType(const mir::MirType* type) {
    VEE_ASSERT(type != nullptr, "Type cannot be null");

    if (type->isVoid()) {
        return "v";
    }
    else if (type->isBool()) {
        return "b";
    }
    else if (auto* integerType = type->asInteger()) {
        switch (integerType->getBitWidth()) {
            case 8:
                return "i8";
            case 16:
                return "i16";
            case 32:
                return "i32";
            case 64:
                return "i64";
            default:
                VEE_UNREACHABLE("Unknown integer bit width");
        }
    }
    if (auto* floatType = type->asFloat()) {
        switch (floatType->getBitWidth()) {
            case 32:
                return "f32";
            case 64:
                return "f64";
            default:
                VEE_UNREACHABLE("Unknown float bit width");
        }
    }
    else if (auto* pointerType = type->asPointer()) {
        return "p" + mangleType(pointerType->getPointeeType());
    }
    else if (auto* arrayType = type->asArray()) {
        return "a" + mangleType(arrayType->getElementType());
    }
    else if (auto* functionType = type->asFunction()) {
        // TODO
        static u32 fnCounter = 0;
        return "ft_" + std::to_string(fnCounter++);
    }
    else if (auto* structType = type->asStruct()) {
        // TODO: Handle generic types
        return "vt_" + std::to_string(structType->getId());
    }

    VEE_UNREACHABLE("Unknown type kind");
}

} // namespace support
} // namespace codegen
VEEC_NAMESPACE_END
