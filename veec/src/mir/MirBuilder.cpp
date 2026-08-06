#include "veec/mir/MirBuilder.hpp"

#include <string>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/MirContext.hpp"
#include "veec/mir/MirFactory.hpp"
#include "veec/mir/Function.hpp"
#include "veec/mir/BasicBlock.hpp"
#include "veec/mir/Instruction.hpp"
#include "veec/mir/Value.hpp"
#include "veec/mir/Constant.hpp"
#include "veec/mir/support/ValueTypeMap.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/types/Type.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/types/BuiltinType.hpp"
#include "veec/types/PointerType.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

//
// Structural node creation
//

Module* MirBuilder::createModule() {
    return _mir.factory.createModule();
}
Function* MirBuilder::createFunction(Module* module, symbols::FunctionSymbol* sym, std::string_view name) {
    return _mir.factory.createFunction(module, sym, name);
}
BasicBlock* MirBuilder::createBasicBlock(Function* function, std::string_view name) {
    return _mir.factory.createBasicBlock(function, name);
}
Local* MirBuilder::createLocal(Function* function, LocalKind kind, symbols::VariableSymbol* sym, std::string_view name) {
    return _mir.factory.createLocal(function, kind, sym, name);
}

ConstantInt* MirBuilder::getConstantInt(Module* module, types::Type* type, basic::APInt value, std::string_view name) {
    return _mir.factory.getConstantInt(module, type, std::move(value), name);
}
ConstantInt* MirBuilder::getConstantIntOne(Module* module, types::Type* type, std::string_view name) {
    return _mir.factory.getConstantIntOne(module, type, name);
}
ConstantFloat* MirBuilder::getConstantFloat(Module* module, types::Type* type, double value, std::string_view name) {
    return _mir.factory.getConstantFloat(module, type, value, name);
}
ConstantString* MirBuilder::getConstantString(Module* module, types::Type* type, std::string_view value, std::string_view name) {
    return _mir.factory.getConstantString(module, type, value, name);
}
ConstantBool* MirBuilder::getConstantBool(Module* module, types::Type* type, bool value, std::string_view name) {
    return _mir.factory.getConstantBool(module, type, value, name);
}

//
// Basic
//

void MirBuilder::createNop() {
    createInstruction(InstructionOpcode::Nop);
}

//
// SSA / value
//

Value* MirBuilder::createPhi(types::Type*, basic::SmallVector<std::pair<Value*, mir::BasicBlock*>>&&) {
    beginInstruction();
    return nullptr; // TODO
}

//
// Integer arithmetic
//

Value* MirBuilder::createNeg(Value* value, std::string_view resultName) {
    assertInteger(value, "value");

    return createValueInstruction(InstructionOpcode::Neg, getValueType(value), { value }, resultName);
}
Value* MirBuilder::createAdd(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::Add, getValueType(lhs), { lhs, rhs }, resultName);
}
Value* MirBuilder::createSub(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::Sub, getValueType(lhs), { lhs, rhs }, resultName);
}
Value* MirBuilder::createMul(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::Mul, getValueType(lhs), { lhs, rhs }, resultName);
}
Value* MirBuilder::createSDiv(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::SDiv, getValueType(lhs), { lhs, rhs }, resultName);
}
Value* MirBuilder::createUDiv(Value* lhs, Value* rhs, std::string_view resultName) {
    assertUnsignedInteger(lhs, "lhs");
    assertUnsignedInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::UDiv, getValueType(lhs), { lhs, rhs }, resultName);
}
Value* MirBuilder::createSMod(Value* lhs, Value* rhs, std::string_view resultName) {
    assertSignedInteger(lhs, "lhs");
    assertSignedInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::SMod, getValueType(lhs), { lhs, rhs }, resultName);
}
Value* MirBuilder::createUMod(Value* lhs, Value* rhs, std::string_view resultName) {
    assertUnsignedInteger(lhs, "lhs");
    assertUnsignedInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::UMod, getValueType(lhs), { lhs, rhs }, resultName);
}

//
// Bitwise
//

Value* MirBuilder::createBitNot(Value* value, std::string_view resultName) {
    assertInteger(value, "value");

    return createValueInstruction(InstructionOpcode::BitNot, getValueType(value), { value }, resultName);
}
Value* MirBuilder::createBitAnd(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::BitAnd, getValueType(lhs), { lhs, rhs }, resultName);
}
Value* MirBuilder::createBitOr(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::BitOr, getValueType(lhs), { lhs, rhs }, resultName);
}
Value* MirBuilder::createBitXor(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::BitXor, getValueType(lhs), { lhs, rhs }, resultName);
}

Value* MirBuilder::createBitShl(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::BitShl, getValueType(lhs), { lhs, rhs }, resultName);
}
Value* MirBuilder::createBitLShr(Value* lhs, Value* rhs, std::string_view resultName) {
    assertUnsignedInteger(lhs, "lhs");
    assertUnsignedInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::BitLShr, getValueType(lhs), { lhs, rhs }, resultName);
}

Value* MirBuilder::createBitAShr(Value* lhs, Value* rhs, std::string_view resultName) {
    assertSignedInteger(lhs, "lhs");
    assertSignedInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::BitAShr, getValueType(lhs), { lhs, rhs }, resultName);
}

//
// Float arithmetic
//

Value* MirBuilder::createFNeg(Value* value, std::string_view resultName) {
    assertFloat(value, "value");

    return createValueInstruction(InstructionOpcode::FNeg, getValueType(value), { value }, resultName);
}
Value* MirBuilder::createFAdd(Value* lhs, Value* rhs, std::string_view resultName) {
    assertFloat(lhs, "lhs");
    assertFloat(rhs, "rhs");
    
    return createValueInstruction(InstructionOpcode::FAdd, getValueType(lhs), { lhs, rhs }, resultName);
}
Value* MirBuilder::createFSub(Value* lhs, Value* rhs, std::string_view resultName) {
    assertFloat(lhs, "lhs");
    assertFloat(rhs, "rhs");
    
    return createValueInstruction(InstructionOpcode::FSub, getValueType(lhs), { lhs, rhs }, resultName);
}
Value* MirBuilder::createFMul(Value* lhs, Value* rhs, std::string_view resultName) {
    assertFloat(lhs, "lhs");
    assertFloat(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::FMul, getValueType(lhs), { lhs, rhs }, resultName);
}
Value* MirBuilder::createFDiv(Value* lhs, Value* rhs, std::string_view resultName) {
    assertFloat(lhs, "lhs");
    assertFloat(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::FDiv, getValueType(lhs), { lhs, rhs }, resultName);
}

//
// Logical
//

Value* MirBuilder::createLogicalNot(Value* value, std::string_view resultName) {
    assertBoolean(value, "value");

    return createValueInstruction(InstructionOpcode::LogicalNot, getBoolType(), { value }, resultName);
}
Value* MirBuilder::createLogicalAnd(Value* lhs, Value* rhs, std::string_view resultName) {
    assertBoolean(lhs, "lhs");
    assertBoolean(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::LogicalAnd, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createLogicalOr(Value* lhs, Value* rhs, std::string_view resultName) {
    assertBoolean(lhs, "lhs");
    assertBoolean(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::LogicalOr, getBoolType(), { lhs, rhs }, resultName);
}

//
// Integer comparison
//

Value* MirBuilder::createICmpEq(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpEq, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpNe(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpNe, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpSlt(Value* lhs, Value* rhs, std::string_view resultName) {
    assertSignedInteger(lhs, "lhs");
    assertSignedInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpSlt, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpSle(Value* lhs, Value* rhs, std::string_view resultName) {
    assertSignedInteger(lhs, "lhs");
    assertSignedInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpSle, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpSgt(Value* lhs, Value* rhs, std::string_view resultName) {
    assertSignedInteger(lhs, "lhs");
    assertSignedInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpSgt, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpSge(Value* lhs, Value* rhs, std::string_view resultName) {
    assertSignedInteger(lhs, "lhs");
    assertSignedInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpSge, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpUlt(Value* lhs, Value* rhs, std::string_view resultName) {
    assertUnsignedInteger(lhs, "lhs");
    assertUnsignedInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpUlt, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpUle(Value* lhs, Value* rhs, std::string_view resultName) {
    assertUnsignedInteger(lhs, "lhs");
    assertUnsignedInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpUle, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpUgt(Value* lhs, Value* rhs, std::string_view resultName) {
    assertUnsignedInteger(lhs, "lhs");
    assertUnsignedInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpUgt, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpUge(Value* lhs, Value* rhs, std::string_view resultName) {
    assertUnsignedInteger(lhs, "lhs");
    assertUnsignedInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpUge, getBoolType(), { lhs, rhs }, resultName);
}

//
// Float comparison
//

Value* MirBuilder::createFCmpEq(Value* lhs, Value* rhs, std::string_view resultName) {
    assertFloat(lhs, "lhs");
    assertFloat(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::FCmpEq, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createFCmpNe(Value* lhs, Value* rhs, std::string_view resultName) {
    assertFloat(lhs, "lhs");
    assertFloat(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::FCmpNe, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createFCmpLt(Value* lhs, Value* rhs, std::string_view resultName) {
    assertFloat(lhs, "lhs");
    assertFloat(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::FCmpLt, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createFCmpLe(Value* lhs, Value* rhs, std::string_view resultName) {
    assertFloat(lhs, "lhs");
    assertFloat(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::FCmpLe, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createFCmpGt(Value* lhs, Value* rhs, std::string_view resultName) {
    assertFloat(lhs, "lhs");
    assertFloat(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::FCmpGt, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createFCmpGe(Value* lhs, Value* rhs, std::string_view resultName) {
    assertFloat(lhs, "lhs");
    assertFloat(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::FCmpGe, getBoolType(), { lhs, rhs }, resultName);
}

//
// Memory & data
//

void MirBuilder::createStore(Value* address, Value* value) {
    VEE_ASSERT(address != nullptr, "Address is null!");
    VEE_ASSERT(value != nullptr, "Value is null!");

    createInstruction(InstructionOpcode::Store, { address, value });
}
Value* MirBuilder::createLoad(Value* address, std::string_view resultName) {
    VEE_ASSERT(address != nullptr, "Address is null!");

    return createValueInstruction(InstructionOpcode::Load, getValueType(address), { address }, resultName);
}

//
// Data
//

Value* MirBuilder::createConstruct(types::Type* type, basic::SmallVector<Value*, 2>&& values, std::string_view resultName) {
    VEE_ASSERT(type != nullptr, "Type is null!");

    return createValueInstruction(InstructionOpcode::Construct, type, { std::move(values) }, resultName);
}

//
// Conversion / casting
//

Value* MirBuilder::createTruncateInt(Value* value, types::Type* toType, std::string_view resultName) {
    assertInteger(value, "value");
    VEE_ASSERT(toType != nullptr, "toType is null!");

    types::BuiltinType* builtinValueType = _mir.valueTypes.getValueType(value)->as<types::BuiltinType>();
    u32 valueBitWidth = builtinValueType->getBitWidth();

    types::BuiltinType* builtinToType = toType->as<types::BuiltinType>();
    VEE_ASSERT(builtinToType->isInteger(), "toType must be an integer type!");
    u32 toBitWidth = builtinToType->getBitWidth();

    VEE_ASSERT(toBitWidth < valueBitWidth, "toType must have a smaller bit width than the value type!");
    VEE_ASSERT(builtinValueType->isSignedInteger() == builtinToType->isSignedInteger(),
        "Invalid integer truncation: from {} to {}. Both types must be of the same signedness",
        builtinValueType->toString(), builtinToType->toString());

    return createValueInstruction(InstructionOpcode::TruncateInt, toType, { value }, resultName);
}
Value* MirBuilder::createExtendInt(Value* value, types::Type* toType, std::string_view resultName) {
    assertInteger(value, "value");
    VEE_ASSERT(toType != nullptr, "toType is null!");

    types::BuiltinType* builtinValueType = _mir.valueTypes.getValueType(value)->as<types::BuiltinType>();
    u32 valueBitWidth = builtinValueType->getBitWidth();

    types::BuiltinType* builtinToType = toType->as<types::BuiltinType>();
    VEE_ASSERT(builtinToType->isInteger(), "toType must be an integer type!");
    u32 toBitWidth = builtinToType->getBitWidth();

    VEE_ASSERT(toBitWidth > valueBitWidth, "toType must have a larger bit width than the value type!");

    if (builtinValueType->isSignedInteger() && builtinToType->isSignedInteger()) {
        return createValueInstruction(InstructionOpcode::SignExtendInt, toType, { value }, resultName);
    } else if (builtinValueType->isUnsignedInteger() && builtinToType->isUnsignedInteger()) {
        return createValueInstruction(InstructionOpcode::ZeroExtendInt, toType, { value }, resultName);
    }

    VEE_FATAL("Invalid integer extension: from {} to {}. Both types must be of the same signedness",
        builtinValueType->toString(), builtinToType->toString());
}

Value* MirBuilder::createTruncateFloat(Value* value, types::Type* toType, std::string_view resultName) {
    assertFloat(value, "value");
    VEE_ASSERT(toType != nullptr, "toType is null!");

    types::BuiltinType* builtinValueType = _mir.valueTypes.getValueType(value)->as<types::BuiltinType>();
    u32 valueBitWidth = builtinValueType->getBitWidth();

    types::BuiltinType* builtinToType = toType->as<types::BuiltinType>();
    VEE_ASSERT(builtinToType->isFloatingPoint(), "toType must be a floating-point type!");
    u32 toBitWidth = builtinToType->getBitWidth();

    VEE_ASSERT(toBitWidth < valueBitWidth, "toType must have a smaller bit width than the value type!");

    return createValueInstruction(InstructionOpcode::TruncateFloat, toType, { value }, resultName);
}
Value* MirBuilder::createExtendFloat(Value* value, types::Type* toType, std::string_view resultName) {
    assertFloat(value, "value");
    VEE_ASSERT(toType != nullptr, "toType is null!");

    types::BuiltinType* builtinValueType = _mir.valueTypes.getValueType(value)->as<types::BuiltinType>();
    u32 valueBitWidth = builtinValueType->getBitWidth();

    types::BuiltinType* builtinToType = toType->as<types::BuiltinType>();
    VEE_ASSERT(builtinToType->isFloatingPoint(), "toType must be a floating-point type!");
    u32 toBitWidth = builtinToType->getBitWidth();

    VEE_ASSERT(toBitWidth > valueBitWidth, "toType must have a larger bit width than the value type!");

    return createValueInstruction(InstructionOpcode::ExtendFloat, toType, { value }, resultName);
}

Value* MirBuilder::createIntToFloat(Value* value, types::Type* toType, std::string_view resultName) {
    assertInteger(value, "value");
    VEE_ASSERT(toType != nullptr, "toType is null!");

    types::BuiltinType* builtinToType = toType->as<types::BuiltinType>();
    VEE_ASSERT(builtinToType->isFloatingPoint(), "toType must be a floating-point type!");

    return createValueInstruction(InstructionOpcode::IntToFloat, toType, { value }, resultName);
}
Value* MirBuilder::createFloatToInt(Value* value, types::Type* toType, std::string_view resultName) {
    assertFloat(value, "value");
    VEE_ASSERT(toType != nullptr, "toType is null!");

    types::BuiltinType* builtinToType = toType->as<types::BuiltinType>();
    VEE_ASSERT(builtinToType->isInteger(), "toType must be an integer type!");

    return createValueInstruction(InstructionOpcode::FloatToInt, toType, { value }, resultName);
}

Value* MirBuilder::createPointerToInt(Value* value, types::Type* toType, std::string_view resultName) {
    assertPointer(value, "value");
    VEE_ASSERT(toType != nullptr, "toType is null!");

    types::BuiltinType* builtinToType = toType->as<types::BuiltinType>();
    VEE_ASSERT(builtinToType->isInteger(), "toType must be an integer type!");

    return createValueInstruction(InstructionOpcode::PtrToInt, toType, { value }, resultName);
}
Value* MirBuilder::createIntToPointer(Value* value, types::Type* toType, std::string_view resultName) {
    assertInteger(value, "value");
    VEE_ASSERT(toType != nullptr, "toType is null!");

    types::PointerType* pointerToType = toType->as<types::PointerType>();
    VEE_ASSERT(pointerToType != nullptr, "toType must be a pointer type!");

    return createValueInstruction(InstructionOpcode::IntToPtr, toType, { value }, resultName);
}

//
// Terminators
//

Value* MirBuilder::createCall(Function* function, const basic::SmallVector<Value*, 2>& args, std::string_view resultName) {
    VEE_ASSERT(function != nullptr, "Function is null!");

    // -> [function, arg1, arg2, ...]
    basic::SmallVector<Value*, 2> operands;
    operands.push_back(function);
    for (Value* arg : args) {
        VEE_ASSERT(arg != nullptr, "Argument is null!");
        operands.push_back(arg);
    }
    
    types::Type* returnType = _ctx.mir.valueTypes.getValueType(function);
    return createValueInstruction(InstructionOpcode::Call, returnType, std::move(operands), resultName);
}
void MirBuilder::createRet(Value* returnValue) {
    VEE_ASSERT(returnValue != nullptr,
        "Return value is null, for void return use createRetVoid() instead");

    createInstruction(InstructionOpcode::Ret, { returnValue });
}
void MirBuilder::createRetVoid() {
    createInstruction(InstructionOpcode::Ret);
}
void MirBuilder::createBranch(mir::BasicBlock* target) {
    createInstruction(InstructionOpcode::Br, { target });
}
void MirBuilder::createConditionalBranch(Value* condition, mir::BasicBlock* trueTarget, mir::BasicBlock* falseTarget) {
    createInstruction(InstructionOpcode::CondBr, { condition, trueTarget, falseTarget });
}
void MirBuilder::createUnreachable() {
    createInstruction(InstructionOpcode::Unreachable);
}

//
// Type checks (assertions)
//

void MirBuilder::assertBoolean(Value* value, std::string_view name) {
    VEE_ASSERT(value != nullptr, "{} is null!", name);
    VEE_ASSERT(_mir.valueTypes.getValueType(value)->is<types::BuiltinType>(), "{} must be of boolean type!", name);
    VEE_ASSERT(_mir.valueTypes.getValueType(value)->as<types::BuiltinType>()->isBoolean(), "{} must be of boolean type!", name);
}
void MirBuilder::assertInteger(Value* value, std::string_view name) {
    VEE_ASSERT(value != nullptr, "{} is null!", name);
    VEE_ASSERT(_mir.valueTypes.getValueType(value)->is<types::BuiltinType>(), "{} must be of integer type!", name);
    VEE_ASSERT(_mir.valueTypes.getValueType(value)->as<types::BuiltinType>()->isInteger(), "{} must be of integer type!", name);
}
void MirBuilder::assertSignedInteger(Value* value, std::string_view name) {
    VEE_ASSERT(value != nullptr, "{} is null!", name);
    VEE_ASSERT(_mir.valueTypes.getValueType(value)->is<types::BuiltinType>(), "{} must be of signed integer type!", name);
    VEE_ASSERT(_mir.valueTypes.getValueType(value)->as<types::BuiltinType>()->isSignedInteger(), "{} must be of signed integer type!", name);
}
void MirBuilder::assertUnsignedInteger(Value* value, std::string_view name) {
    VEE_ASSERT(value != nullptr, "{} is null!", name);
    VEE_ASSERT(_mir.valueTypes.getValueType(value)->is<types::BuiltinType>(), "{} must be of unsigned integer type!", name);
    VEE_ASSERT(_mir.valueTypes.getValueType(value)->as<types::BuiltinType>()->isUnsignedInteger(), "{} must be of unsigned integer type!", name);
}
void MirBuilder::assertFloat(Value* value, std::string_view name) {
    VEE_ASSERT(value != nullptr, "{} is null!", name);
    VEE_ASSERT(_mir.valueTypes.getValueType(value)->is<types::BuiltinType>(), "{} must be of float type!", name);
    VEE_ASSERT(_mir.valueTypes.getValueType(value)->as<types::BuiltinType>()->isFloatingPoint(), "{} must be of float type!", name);
}
void MirBuilder::assertPointer(Value* value, std::string_view name) {
    VEE_ASSERT(value != nullptr, "{} is null!", name);
    VEE_ASSERT(_mir.valueTypes.getValueType(value)->is<types::PointerType>(), "{} must be of pointer type!", name);
}

void MirBuilder::createInstruction(InstructionOpcode opcode) {
    beginInstruction();
    Instruction* inst = _mir.factory.createInstruction(opcode);
    finishInstruction(inst);
}
void MirBuilder::createInstruction(InstructionOpcode opcode, basic::SmallVector<Value*, 2>&& operands) {
    beginInstruction();
    Instruction* inst = _mir.factory.createInstruction(opcode, std::move(operands));
    finishInstruction(inst);
}
Value* MirBuilder::createValueInstruction(InstructionOpcode opcode, types::Type* type, basic::SmallVector<Value*, 2>&& operands, std::string_view resultName) {
    beginInstruction();
    Instruction* inst = _mir.factory.createInstruction(opcode, std::move(operands));
    _ctx.mir.valueTypes.setValueType(inst, type);
    finishInstruction(inst);
    if (!resultName.empty()) {
        nameValue(inst, resultName);
    }
    return inst;
}
void MirBuilder::beginInstruction() {
    insertGuard();
}
void MirBuilder::finishInstruction(Instruction* inst) {
    insertGuard();
    VEE_ASSERT(inst != nullptr, "Instruction is null!");

    _insertBlock->addInstruction(inst);
}

void MirBuilder::nameValue(const Value* val, std::string_view name) {
    VEE_ASSERT(val != nullptr, "Value is null!");
    VEE_ASSERT(!name.empty(), "Name is empty!");

    _mir.valueNames.setName(val, name);
}

types::Type* MirBuilder::getValueType(const Value* value) {
    VEE_ASSERT(value != nullptr, "Value is null!");
    types::Type* type = _mir.valueTypes.getValueType(value);
    VEE_ASSERT(type != nullptr, "Value has no type!");
    return type;
}
types::Type* MirBuilder::getBoolType() {
    return _ctx.types.table.getBuiltin(types::BuiltinTypeKind::Bool);
}

} // namespace mir
VEEC_NAMESPACE_END
