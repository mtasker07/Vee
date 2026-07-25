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

std::span<const ConversionRule> TypeSystem::getBuiltinConversionRules() const {
    Type* boolTy = _typeTable.getBuiltin(BuiltinTypeKind::BOOL);
    Type* i8Ty = _typeTable.getBuiltin(BuiltinTypeKind::I8);
    Type* i16Ty = _typeTable.getBuiltin(BuiltinTypeKind::I16);
    Type* i32Ty = _typeTable.getBuiltin(BuiltinTypeKind::I32);
    Type* i64Ty = _typeTable.getBuiltin(BuiltinTypeKind::I64);
    Type* u8Ty = _typeTable.getBuiltin(BuiltinTypeKind::U8);
    Type* u16Ty = _typeTable.getBuiltin(BuiltinTypeKind::U16);
    Type* u32Ty = _typeTable.getBuiltin(BuiltinTypeKind::U32);
    Type* u64Ty = _typeTable.getBuiltin(BuiltinTypeKind::U64);
    Type* f32Ty = _typeTable.getBuiltin(BuiltinTypeKind::F32);
    Type* f64Ty = _typeTable.getBuiltin(BuiltinTypeKind::F64);
    
    static std::vector<ConversionRule> rules = {
        // ------------------------------------------------------------
        // Promotions
        // ------------------------------------------------------------

        // Signed integer widening
        { i8Ty,  i16Ty, ConversionRank::Promotion },
        { i8Ty,  i32Ty, ConversionRank::Promotion },
        { i8Ty,  i64Ty, ConversionRank::Promotion },

        { i16Ty, i32Ty, ConversionRank::Promotion },
        { i16Ty, i64Ty, ConversionRank::Promotion },

        { i32Ty, i64Ty, ConversionRank::Promotion },

        // Unsigned integer widening
        { u8Ty,  u16Ty, ConversionRank::Promotion },
        { u8Ty,  u32Ty, ConversionRank::Promotion },
        { u8Ty,  u64Ty, ConversionRank::Promotion },

        { u16Ty, u32Ty, ConversionRank::Promotion },
        { u16Ty, u64Ty, ConversionRank::Promotion },

        { u32Ty, u64Ty, ConversionRank::Promotion },

        // Float widening
        { f32Ty, f64Ty, ConversionRank::Promotion },

        // ------------------------------------------------------------
        // Conversions
        // ------------------------------------------------------------

        // Integer -> floating point
        { i8Ty,  f32Ty, ConversionRank::Conversion },
        { i16Ty, f32Ty, ConversionRank::Conversion },
        { i32Ty, f32Ty, ConversionRank::Conversion },
        { i64Ty, f64Ty, ConversionRank::Conversion },

        { u8Ty,  f32Ty, ConversionRank::Conversion },
        { u16Ty, f32Ty, ConversionRank::Conversion },
        { u32Ty, f32Ty, ConversionRank::Conversion },
        { u64Ty, f64Ty, ConversionRank::Conversion },

        // Signed <-> unsigned (same width)
        { i8Ty,  u8Ty,  ConversionRank::Conversion },
        { i16Ty, u16Ty, ConversionRank::Conversion },
        { i32Ty, u32Ty, ConversionRank::Conversion },
        { i64Ty, u64Ty, ConversionRank::Conversion },

        { u8Ty,  i8Ty,  ConversionRank::Conversion },
        { u16Ty, i16Ty, ConversionRank::Conversion },
        { u32Ty, i32Ty, ConversionRank::Conversion },
        { u64Ty, i64Ty, ConversionRank::Conversion },

        // ------------------------------------------------------------
        // Narrowing conversions
        // ------------------------------------------------------------

        // Signed narrowing
        { i16Ty, i8Ty,  ConversionRank::NarrowingConversion },
        { i32Ty, i8Ty,  ConversionRank::NarrowingConversion },
        { i32Ty, i16Ty, ConversionRank::NarrowingConversion },
        { i64Ty, i8Ty,  ConversionRank::NarrowingConversion },
        { i64Ty, i16Ty, ConversionRank::NarrowingConversion },
        { i64Ty, i32Ty, ConversionRank::NarrowingConversion },

        // Unsigned narrowing
        { u16Ty, u8Ty,  ConversionRank::NarrowingConversion },
        { u32Ty, u8Ty,  ConversionRank::NarrowingConversion },
        { u32Ty, u16Ty, ConversionRank::NarrowingConversion },
        { u64Ty, u8Ty,  ConversionRank::NarrowingConversion },
        { u64Ty, u16Ty, ConversionRank::NarrowingConversion },
        { u64Ty, u32Ty, ConversionRank::NarrowingConversion },

        // Float narrowing
        { f64Ty, f32Ty, ConversionRank::NarrowingConversion },

        // Float -> integer
        { f32Ty, i8Ty,  ConversionRank::NarrowingConversion },
        { f32Ty, i16Ty, ConversionRank::NarrowingConversion },
        { f32Ty, i32Ty, ConversionRank::NarrowingConversion },
        { f32Ty, i64Ty, ConversionRank::NarrowingConversion },

        { f64Ty, i8Ty,  ConversionRank::NarrowingConversion },
        { f64Ty, i16Ty, ConversionRank::NarrowingConversion },
        { f64Ty, i32Ty, ConversionRank::NarrowingConversion },
        { f64Ty, i64Ty, ConversionRank::NarrowingConversion },

        { f32Ty, u8Ty,  ConversionRank::NarrowingConversion },
        { f32Ty, u16Ty, ConversionRank::NarrowingConversion },
        { f32Ty, u32Ty, ConversionRank::NarrowingConversion },
        { f32Ty, u64Ty, ConversionRank::NarrowingConversion },

        { f64Ty, u8Ty,  ConversionRank::NarrowingConversion },
        { f64Ty, u16Ty, ConversionRank::NarrowingConversion },
        { f64Ty, u32Ty, ConversionRank::NarrowingConversion },
        { f64Ty, u64Ty, ConversionRank::NarrowingConversion },

        // ------------------------------------------------------------
        // Bool conversions
        // ------------------------------------------------------------

        { boolTy, i32Ty, ConversionRank::Promotion },
        { boolTy, u32Ty, ConversionRank::Promotion },

        { i32Ty, boolTy, ConversionRank::Conversion },
        { u32Ty, boolTy, ConversionRank::Conversion },
    };

    return rules;
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

    // Check builtin conversion rules
    for (const auto& rule : getBuiltinConversionRules()) {
        if (rule.from == from && rule.to == to) {
            return rule.rank;
        }
    }

    // TODO: Add user-defined conversion rules

    return ConversionRank::NoConversion;
}

} // namespace types
VEEC_NAMESPACE_END
