#include "veec/sema/BuiltinRegistrar.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/sema/SemaContext.hpp"
#include "veec/symbols/OperatorTable.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/types/Type.hpp"
#include "veec/types/BuiltinType.hpp"
#include "veec/types/TypeTable.hpp"
#include "veec/types/TypeSystem.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema {

void BuiltinRegistrar::registerAll() {
    registerBuiltinOperators();
    registerBuiltinConversions();
}

void BuiltinRegistrar::registerBuiltinOperators() {
    using Type = types::Type;
    using Builtin = types::BuiltinTypeKind;
    using UnaryOp = symbols::UnaryOperatorKind;
    using BinaryOp = symbols::BinaryOperatorKind;

    Type* boolTy = _sema.types.getBuiltin(Builtin::Bool);
    Type* i8Ty   = _sema.types.getBuiltin(Builtin::I8);
    Type* i16Ty  = _sema.types.getBuiltin(Builtin::I16);
    Type* i32Ty  = _sema.types.getBuiltin(Builtin::I32);
    Type* i64Ty  = _sema.types.getBuiltin(Builtin::I64);
    Type* u8Ty   = _sema.types.getBuiltin(Builtin::U8);
    Type* u16Ty  = _sema.types.getBuiltin(Builtin::U16);
    Type* u32Ty  = _sema.types.getBuiltin(Builtin::U32);
    Type* u64Ty  = _sema.types.getBuiltin(Builtin::U64);
    Type* f32Ty  = _sema.types.getBuiltin(Builtin::F32);
    Type* f64Ty  = _sema.types.getBuiltin(Builtin::F64);

    auto unary = [&, this](UnaryOp op, Type* operand, Type* result) {
        auto* opSymbol = _sema.symbols.declare<symbols::OperatorSymbol>(
            symbols::OperatorImplementation::Builtin,
            op,
            result,
            std::vector<Type*>{ operand }
        );

        _sema.operators.addOperator(opSymbol);
    };

    auto binary = [&, this](BinaryOp op, Type* lhs, Type* rhs, Type* result) {
        auto* opSymbol = _sema.symbols.declare<symbols::OperatorSymbol>(
            symbols::OperatorImplementation::Builtin,
            op,
            result,
            std::vector<Type*>{ lhs, rhs }
        );

        _sema.operators.addOperator(opSymbol);
    };

    auto arithmetic = [&](Type* ty) {
        binary(BinaryOp::Add,      ty, ty, ty);
        binary(BinaryOp::Subtract, ty, ty, ty);
        binary(BinaryOp::Multiply, ty, ty, ty);
        binary(BinaryOp::Divide,   ty, ty, ty);

        unary(UnaryOp::Plus,  ty, ty);
        unary(UnaryOp::Minus, ty, ty);

        binary(BinaryOp::Equal,              ty, ty, boolTy);
        binary(BinaryOp::NotEqual,           ty, ty, boolTy);
        binary(BinaryOp::LessThan,           ty, ty, boolTy);
        binary(BinaryOp::LessThanOrEqual,    ty, ty, boolTy);
        binary(BinaryOp::GreaterThan,        ty, ty, boolTy);
        binary(BinaryOp::GreaterThanOrEqual, ty, ty, boolTy);
    };

    auto integral = [&](Type* ty) {
        arithmetic(ty);

        binary(BinaryOp::Modulo,     ty, ty, ty);

        binary(BinaryOp::BitwiseAnd, ty, ty, ty);
        binary(BinaryOp::BitwiseOr,  ty, ty, ty);
        binary(BinaryOp::BitwiseXor, ty, ty, ty);

        binary(BinaryOp::ShiftLeft,  ty, ty, ty);
        binary(BinaryOp::ShiftRight, ty, ty, ty);

        unary(UnaryOp::BitwiseNot, ty, ty);
    };

    auto floating = [&](Type* ty) {
        arithmetic(ty);
    };

    auto logical = [&](Type* ty) {
        unary(UnaryOp::LogicalNot, ty, boolTy);

        binary(BinaryOp::LogicalAnd, ty, ty, boolTy);
        binary(BinaryOp::LogicalOr,  ty, ty, boolTy);

        binary(BinaryOp::Equal,    ty, ty, boolTy);
        binary(BinaryOp::NotEqual, ty, ty, boolTy);
    };

    // Signed integers
    integral(i8Ty);
    integral(i16Ty);
    integral(i32Ty);
    integral(i64Ty);

    // Unsigned integers
    integral(u8Ty);
    integral(u16Ty);
    integral(u32Ty);
    integral(u64Ty);

    // Floating point
    floating(f32Ty);
    floating(f64Ty);

    // Bool
    logical(boolTy);
}
void BuiltinRegistrar::registerBuiltinConversions() {
    using Type = types::Type;
    using Builtin = types::BuiltinTypeKind;
    using Rank = types::ConversionRank;

    Type* boolTy = _sema.types.getBuiltin(Builtin::Bool);
    Type* i8Ty   = _sema.types.getBuiltin(Builtin::I8);
    Type* i16Ty  = _sema.types.getBuiltin(Builtin::I16);
    Type* i32Ty  = _sema.types.getBuiltin(Builtin::I32);
    Type* i64Ty  = _sema.types.getBuiltin(Builtin::I64);
    Type* u8Ty   = _sema.types.getBuiltin(Builtin::U8);
    Type* u16Ty  = _sema.types.getBuiltin(Builtin::U16);
    Type* u32Ty  = _sema.types.getBuiltin(Builtin::U32);
    Type* u64Ty  = _sema.types.getBuiltin(Builtin::U64);
    Type* f32Ty  = _sema.types.getBuiltin(Builtin::F32);
    Type* f64Ty  = _sema.types.getBuiltin(Builtin::F64);

    auto promote = [this](Type* from, Type* to) {
        _sema.typeSystem.addConversionRule(from, to, Rank::Promotion);
    };

    auto convert = [this](Type* from, Type* to) {
        _sema.typeSystem.addConversionRule(from, to, Rank::Conversion);
    };

    auto narrow = [this](Type* from, Type* to) {
        _sema.typeSystem.addConversionRule(from, to, Rank::NarrowingConversion);
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

} // namespace sema
VEEC_NAMESPACE_END
