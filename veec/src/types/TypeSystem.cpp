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

void TypeSystem::addBuiltinConversionRules() {
    Type* boolTy = _typeTable.getBuiltin(BuiltinTypeKind::Bool);
    Type* i8Ty   = _typeTable.getBuiltin(BuiltinTypeKind::I8);
    Type* i16Ty  = _typeTable.getBuiltin(BuiltinTypeKind::I16);
    Type* i32Ty  = _typeTable.getBuiltin(BuiltinTypeKind::I32);
    Type* i64Ty  = _typeTable.getBuiltin(BuiltinTypeKind::I64);
    Type* u8Ty   = _typeTable.getBuiltin(BuiltinTypeKind::U8);
    Type* u16Ty  = _typeTable.getBuiltin(BuiltinTypeKind::U16);
    Type* u32Ty  = _typeTable.getBuiltin(BuiltinTypeKind::U32);
    Type* u64Ty  = _typeTable.getBuiltin(BuiltinTypeKind::U64);
    Type* f32Ty  = _typeTable.getBuiltin(BuiltinTypeKind::F32);
    Type* f64Ty  = _typeTable.getBuiltin(BuiltinTypeKind::F64);

    auto promote = [this](Type* from, Type* to) {
        addConversionRule(from, to, ConversionRank::Promotion);
    };

    auto convert = [this](Type* from, Type* to) {
        addConversionRule(from, to, ConversionRank::Conversion);
    };

    auto narrow = [this](Type* from, Type* to) {
        addConversionRule(from, to, ConversionRank::NarrowingConversion);
    };

    // ------------------------------------------------------------
    // Promotions
    // ------------------------------------------------------------

    // Signed integer widening
    promote(i8Ty,  i16Ty);
    promote(i8Ty,  i32Ty);
    promote(i8Ty,  i64Ty);

    promote(i16Ty, i32Ty);
    promote(i16Ty, i64Ty);

    promote(i32Ty, i64Ty);

    // Unsigned integer widening
    promote(u8Ty,  u16Ty);
    promote(u8Ty,  u32Ty);
    promote(u8Ty,  u64Ty);

    promote(u16Ty, u32Ty);
    promote(u16Ty, u64Ty);

    promote(u32Ty, u64Ty);

    // Float widening
    promote(f32Ty, f64Ty);

    // ------------------------------------------------------------
    // Conversions
    // ------------------------------------------------------------

    // Integer -> floating point
    convert(i8Ty,  f32Ty);
    convert(i16Ty, f32Ty);
    convert(i32Ty, f32Ty);
    convert(i64Ty, f64Ty);

    convert(u8Ty,  f32Ty);
    convert(u16Ty, f32Ty);
    convert(u32Ty, f32Ty);
    convert(u64Ty, f64Ty);

    // Signed <-> unsigned (same width)
    convert(i8Ty,  u8Ty);
    convert(i16Ty, u16Ty);
    convert(i32Ty, u32Ty);
    convert(i64Ty, u64Ty);

    convert(u8Ty,  i8Ty);
    convert(u16Ty, i16Ty);
    convert(u32Ty, i32Ty);
    convert(u64Ty, i64Ty);

    // ------------------------------------------------------------
    // Narrowing conversions
    // ------------------------------------------------------------

    // Signed narrowing
    narrow(i16Ty, i8Ty);
    narrow(i32Ty, i8Ty);
    narrow(i32Ty, i16Ty);
    narrow(i64Ty, i8Ty);
    narrow(i64Ty, i16Ty);
    narrow(i64Ty, i32Ty);

    // Unsigned narrowing
    narrow(u16Ty, u8Ty);
    narrow(u32Ty, u8Ty);
    narrow(u32Ty, u16Ty);
    narrow(u64Ty, u8Ty);
    narrow(u64Ty, u16Ty);
    narrow(u64Ty, u32Ty);

    // Float narrowing
    narrow(f64Ty, f32Ty);

    // Float -> integer
    narrow(f32Ty, i8Ty);
    narrow(f32Ty, i16Ty);
    narrow(f32Ty, i32Ty);
    narrow(f32Ty, i64Ty);

    narrow(f64Ty, i8Ty);
    narrow(f64Ty, i16Ty);
    narrow(f64Ty, i32Ty);
    narrow(f64Ty, i64Ty);

    narrow(f32Ty, u8Ty);
    narrow(f32Ty, u16Ty);
    narrow(f32Ty, u32Ty);
    narrow(f32Ty, u64Ty);

    narrow(f64Ty, u8Ty);
    narrow(f64Ty, u16Ty);
    narrow(f64Ty, u32Ty);
    narrow(f64Ty, u64Ty);

    // ------------------------------------------------------------
    // Bool conversions
    // ------------------------------------------------------------

    promote(boolTy, i32Ty);
    promote(boolTy, u32Ty);

    convert(i32Ty, boolTy);
    convert(u32Ty, boolTy);
}
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
