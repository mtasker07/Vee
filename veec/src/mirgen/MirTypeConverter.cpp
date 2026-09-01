#include "veec/mirgen/MirTypeConverter.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/types/Type.hpp"
#include "veec/types/BuiltinType.hpp"
#include "veec/types/PointerType.hpp"
#include "veec/types/ArrayType.hpp"
#include "veec/types/ClassType.hpp"
#include "veec/mir/MirContext.hpp"
#include "veec/mir/MirType.hpp"

VEEC_NAMESPACE_BEGIN
namespace mirgen {

const mir::MirType* MirTypeConverter::convert(const types::Type* type) {
    VEE_ASSERT(type != nullptr, "Type cannot be null");

    switch (type->getKind()) {
        case types::TypeKind::Builtin:
            return convertBuiltinType(static_cast<const types::BuiltinType*>(type));
        case types::TypeKind::Pointer:
            return convertPointerType(static_cast<const types::PointerType*>(type));
        case types::TypeKind::Array:
            return convertArrayType(static_cast<const types::ArrayType*>(type));
        case types::TypeKind::Class:
            return convertStructType(static_cast<const types::ClassType*>(type));

        default:
            VEE_UNREACHABLE("Unsupported type kind");
    }
}

const mir::MirType* MirTypeConverter::convertBuiltinType(const types::BuiltinType* type) {
    VEE_ASSERT(type != nullptr, "Builtin type cannot be null");

    switch (type->getBuiltinKind()) {
        case types::BuiltinTypeKind::Void:
            return _ctx.mir.types.getVoid();
        case types::BuiltinTypeKind::Bool:
            return _ctx.mir.types.getBool();

        case types::BuiltinTypeKind::String:
            return _ctx.mir.types.getStringStruct();
        
        case types::BuiltinTypeKind::I8:
            return _ctx.mir.types.getInteger(8, true);
        case types::BuiltinTypeKind::I16:
            return _ctx.mir.types.getInteger(16, true);
        case types::BuiltinTypeKind::I32:
            return _ctx.mir.types.getInteger(32, true);
        case types::BuiltinTypeKind::I64:
            return _ctx.mir.types.getInteger(64, true);

        case types::BuiltinTypeKind::U8:
            return _ctx.mir.types.getInteger(8, false);
        case types::BuiltinTypeKind::U16:
            return _ctx.mir.types.getInteger(16, false);
        case types::BuiltinTypeKind::U32:
            return _ctx.mir.types.getInteger(32, false);
        case types::BuiltinTypeKind::U64:
            return _ctx.mir.types.getInteger(64, false);

        case types::BuiltinTypeKind::F32:
            return _ctx.mir.types.getFloat(32);
        case types::BuiltinTypeKind::F64:
            return _ctx.mir.types.getFloat(64);

        default:
            VEE_UNREACHABLE("Unsupported builtin type kind");
    }
}
const mir::MirPointerType* MirTypeConverter::convertPointerType(const types::PointerType* type) {
    VEE_ASSERT(type != nullptr, "Pointer type cannot be null");

    const mir::MirType* pointeeType = convert(type->getPointeeType());
    return _ctx.mir.types.getPointer(pointeeType);
}
const mir::MirArrayType* MirTypeConverter::convertArrayType(const types::ArrayType* type) {
    VEE_ASSERT(type != nullptr, "Array type cannot be null");

    const mir::MirType* elementType = convert(type->getElementType());
    return _ctx.mir.types.getArray(elementType, type->getSize());
}
const mir::MirStructType* MirTypeConverter::convertStructType(const types::ClassType* type) {
    VEE_ASSERT(type != nullptr, "Class type cannot be null");

    // Create opaque stuct
    mir::MirStructType* structType = _ctx.mir.types.createOpaqueStruct();

    // Add members
    for (const auto [nameId, fieldSym] : type->getFields()) {
        const mir::MirType* fieldType = convert(fieldSym->getType());
        structType->addMemberType(fieldType);
    }

    return structType;
}

} // namespace mirgen
VEEC_NAMESPACE_END
