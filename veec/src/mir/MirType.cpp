#include "veec/mir/MirType.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

const MirVoidType* MirType::asVoid() const {
    return isOfKind(MirTypeKind::Void) ? static_cast<const MirVoidType*>(this) : nullptr;
}
const MirBoolType* MirType::asBool() const {
    return isOfKind(MirTypeKind::Bool) ? static_cast<const MirBoolType*>(this) : nullptr;
}
const MirIntegerType* MirType::asInteger() const {
    return isOfKind(MirTypeKind::Integer) ? static_cast<const MirIntegerType*>(this) : nullptr;
}
const MirFloatType* MirType::asFloat() const {
    return isOfKind(MirTypeKind::Float) ? static_cast<const MirFloatType*>(this) : nullptr;
}
const MirPointerType* MirType::asPointer() const {
    return isOfKind(MirTypeKind::Pointer) ? static_cast<const MirPointerType*>(this) : nullptr;
}
const MirArrayType* MirType::asArray() const {
    return isOfKind(MirTypeKind::Array) ? static_cast<const MirArrayType*>(this) : nullptr;
}
const MirStructType* MirType::asStruct() const {
    return isOfKind(MirTypeKind::Struct) ? static_cast<const MirStructType*>(this) : nullptr;
}
const MirFunctionType* MirType::asFunction() const {
    return isOfKind(MirTypeKind::Function) ? static_cast<const MirFunctionType*>(this) : nullptr;
}

MirVoidType* MirType::asVoid() {
    return isOfKind(MirTypeKind::Void) ? static_cast<MirVoidType*>(this) : nullptr;
}
MirBoolType* MirType::asBool() {
    return isOfKind(MirTypeKind::Bool) ? static_cast<MirBoolType*>(this) : nullptr;
}
MirIntegerType* MirType::asInteger() {
    return isOfKind(MirTypeKind::Integer) ? static_cast<MirIntegerType*>(this) : nullptr;
}
MirFloatType* MirType::asFloat() {
    return isOfKind(MirTypeKind::Float) ? static_cast<MirFloatType*>(this) : nullptr;
}
MirPointerType* MirType::asPointer() {
    return isOfKind(MirTypeKind::Pointer) ? static_cast<MirPointerType*>(this) : nullptr;
}
MirArrayType* MirType::asArray() {
    return isOfKind(MirTypeKind::Array) ? static_cast<MirArrayType*>(this) : nullptr;
}
MirStructType* MirType::asStruct() {
    return isOfKind(MirTypeKind::Struct) ? static_cast<MirStructType*>(this) : nullptr;
}
MirFunctionType* MirType::asFunction() {
    return isOfKind(MirTypeKind::Function) ? static_cast<MirFunctionType*>(this) : nullptr;
}

u32 MirType::getBitWidth() const {
    if (auto* intType = asInteger()) {
        return intType->getBitWidth();
    }
    if (auto* floatType = asFloat()) {
        return floatType->getBitWidth();
    }
    return 0;
}

bool MirType::isSigned() const {
    if (auto* intType = asInteger()) {
        return intType->isSigned();
    }
    return false;
}

} // namespace mir
VEEC_NAMESPACE_END
