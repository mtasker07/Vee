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
#include "veec/mir/MirType.hpp"
#include "veec/mir/support/TypeTable.hpp"
#include "veec/symbols/SymbolFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

//
// Structural node creation
//

Module* MirBuilder::createModule() {
    return _mir.factory.createModule();
}
Function* MirBuilder::createFunction(
    Module* module,
    const MirFunctionType* type,
    std::string_view name
) {
    return _mir.factory.createFunction(module, type, name);
}
BasicBlock* MirBuilder::createBasicBlock(
    Function* function,
    std::string_view name
) {
    return _mir.factory.createBasicBlock(function, name);
}

ConstantInt* MirBuilder::getConstantInt(
    Module* module,
    const MirType* type,
    basic::APInt value,
    std::string_view name
) {
    return _mir.factory.getConstantInt(module, type, std::move(value), name);
}
ConstantInt* MirBuilder::getConstantIntOne(
    Module* module,
    const MirType* type,
    std::string_view name
) {
    return _mir.factory.getConstantIntOne(module, type, name);
}
ConstantFloat* MirBuilder::getConstantFloat(
    Module* module,
    const MirType* type,
    double value,
    std::string_view name
) {
    return _mir.factory.getConstantFloat(module, type, value, name);
}
ConstantString* MirBuilder::getConstantString(
    Module* module,
    const MirType* type,
    std::string_view value,
    std::string_view name
) {
    return _mir.factory.getConstantString(module, type, value, name);
}
ConstantBool* MirBuilder::getConstantBool(
    Module* module,
    const MirType* type,
    bool value,
    std::string_view name
) {
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

Value* MirBuilder::createPhi(
    const MirType*,
    basic::SmallVector<std::pair<Value*, mir::BasicBlock*>>&&
) {
    beginInstruction();
    return nullptr; // TODO
}

//
// Integer arithmetic
//

Value* MirBuilder::createNeg(Value* value, std::string_view resultName) {
    assertInteger(value, "value");

    return createValueInstruction(InstructionOpcode::Neg, value->getType(), { value }, resultName);
}
Value* MirBuilder::createAdd(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::Add, lhs->getType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createSub(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::Sub, lhs->getType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createMul(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::Mul, lhs->getType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createSDiv(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::SDiv, lhs->getType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createUDiv(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::UDiv, lhs->getType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createSMod(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::SMod, lhs->getType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createUMod(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::UMod, lhs->getType(), { lhs, rhs }, resultName);
}

//
// Bitwise
//

Value* MirBuilder::createBitNot(Value* value, std::string_view resultName) {
    assertInteger(value, "value");

    return createValueInstruction(InstructionOpcode::BitNot, value->getType(), { value }, resultName);
}
Value* MirBuilder::createBitAnd(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::BitAnd, lhs->getType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createBitOr(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::BitOr, lhs->getType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createBitXor(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::BitXor, lhs->getType(), { lhs, rhs }, resultName);
}

Value* MirBuilder::createBitShl(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::BitShl, lhs->getType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createBitLShr(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::BitLShr, lhs->getType(), { lhs, rhs }, resultName);
}

Value* MirBuilder::createBitAShr(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::BitAShr, lhs->getType(), { lhs, rhs }, resultName);
}

//
// Float arithmetic
//

Value* MirBuilder::createFNeg(Value* value, std::string_view resultName) {
    assertFloat(value, "value");

    return createValueInstruction(InstructionOpcode::FNeg, value->getType(), { value }, resultName);
}
Value* MirBuilder::createFAdd(Value* lhs, Value* rhs, std::string_view resultName) {
    assertFloat(lhs, "lhs");
    assertFloat(rhs, "rhs");
    
    return createValueInstruction(InstructionOpcode::FAdd, lhs->getType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createFSub(Value* lhs, Value* rhs, std::string_view resultName) {
    assertFloat(lhs, "lhs");
    assertFloat(rhs, "rhs");
    
    return createValueInstruction(InstructionOpcode::FSub, lhs->getType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createFMul(Value* lhs, Value* rhs, std::string_view resultName) {
    assertFloat(lhs, "lhs");
    assertFloat(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::FMul, lhs->getType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createFDiv(Value* lhs, Value* rhs, std::string_view resultName) {
    assertFloat(lhs, "lhs");
    assertFloat(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::FDiv, lhs->getType(), { lhs, rhs }, resultName);
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
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpSlt, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpSle(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpSle, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpSgt(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpSgt, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpSge(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpSge, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpUlt(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpUlt, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpUle(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpUle, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpUgt(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

    return createValueInstruction(InstructionOpcode::ICmpUgt, getBoolType(), { lhs, rhs }, resultName);
}
Value* MirBuilder::createICmpUge(Value* lhs, Value* rhs, std::string_view resultName) {
    assertInteger(lhs, "lhs");
    assertInteger(rhs, "rhs");

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

    const MirType* loadedType = address->getType()->asPointer()->getPointeeType();
    return createValueInstruction(InstructionOpcode::Load, loadedType, { address }, resultName);
}

//
// Data
//

Value* MirBuilder::createConstruct(const MirType* type, basic::SmallVector<Value*, 2>&& values, std::string_view resultName) {
    VEE_ASSERT(type != nullptr, "Type is null!");

    return createValueInstruction(InstructionOpcode::Construct, type, { std::move(values) }, resultName);
}

//
// Conversion / casting
//

Value* MirBuilder::createTruncateInt(Value* value, const MirType* toType, std::string_view resultName) {
    assertInteger(value, "value");
    assertIntegerType(toType, "toType");

    const MirIntegerType* integerValueType = value->getType()->asInteger();
    u32 valueBitWidth = integerValueType->getBitWidth();

    const MirIntegerType* integerToType = toType->asInteger();
    u32 toBitWidth = integerToType->getBitWidth();

    VEE_ASSERT(toBitWidth < valueBitWidth, "toType must have a smaller bit width than the value type!");

    return createValueInstruction(InstructionOpcode::TruncateInt, toType, { value }, resultName);
}
Value* MirBuilder::createZExtendInt(Value* value, const MirType* toType, std::string_view resultName) {
    assertInteger(value, "value");
    assertIntegerType(toType, "toType");

    const MirIntegerType* integerValueType = value->getType()->asInteger();
    u32 valueBitWidth = integerValueType->getBitWidth();

    const MirIntegerType* integerToType = toType->asInteger();
    u32 toBitWidth = integerToType->getBitWidth();

    VEE_ASSERT(toBitWidth > valueBitWidth, "toType must have a larger bit width than the value type!");

    return createValueInstruction(InstructionOpcode::ZeroExtendInt, toType, { value }, resultName);
}
Value* MirBuilder::createSExtendInt(Value* value, const MirType* toType, std::string_view resultName) {
    assertInteger(value, "value");
    assertIntegerType(toType, "toType");

    const MirIntegerType* integerValueType = value->getType()->asInteger();
    u32 valueBitWidth = integerValueType->getBitWidth();

    const MirIntegerType* integerToType = toType->asInteger();
    u32 toBitWidth = integerToType->getBitWidth();

    VEE_ASSERT(toBitWidth > valueBitWidth, "toType must have a larger bit width than the value type!");

    return createValueInstruction(InstructionOpcode::SignExtendInt, toType, { value }, resultName);
}

Value* MirBuilder::createTruncateFloat(Value* value, const MirType* toType, std::string_view resultName) {
    assertFloat(value, "value");
    assertFloatType(toType, "toType");

    const MirFloatType* floatValueType = value->getType()->asFloat();
    u32 valueBitWidth = floatValueType->getBitWidth();

    const MirFloatType* floatToType = toType->asFloat();
    u32 toBitWidth = floatToType->getBitWidth();

    VEE_ASSERT(toBitWidth < valueBitWidth, "toType must have a smaller bit width than the value type!");

    return createValueInstruction(InstructionOpcode::TruncateFloat, toType, { value }, resultName);
}
Value* MirBuilder::createExtendFloat(Value* value, const MirType* toType, std::string_view resultName) {
    assertFloat(value, "value");
    assertFloatType(toType, "toType");

    const MirFloatType* floatValueType = value->getType()->asFloat();
    u32 valueBitWidth = floatValueType->getBitWidth();

    const MirFloatType* floatToType = toType->asFloat();
    u32 toBitWidth = floatToType->getBitWidth();

    VEE_ASSERT(toBitWidth > valueBitWidth, "toType must have a larger bit width than the value type!");

    return createValueInstruction(InstructionOpcode::ExtendFloat, toType, { value }, resultName);
}

Value* MirBuilder::createIntToFloat(Value* value, const MirType* toType, std::string_view resultName) {
    assertInteger(value, "value");
    assertFloatType(toType, "toType");

    return createValueInstruction(InstructionOpcode::IntToFloat, toType, { value }, resultName);
}
Value* MirBuilder::createFloatToInt(Value* value, const MirType* toType, std::string_view resultName) {
    assertFloat(value, "value");
    assertIntegerType(toType, "toType");

    return createValueInstruction(InstructionOpcode::FloatToInt, toType, { value }, resultName);
}

Value* MirBuilder::createPointerToInt(Value* value, const MirType* toType, std::string_view resultName) {
    assertPointer(value, "value");
    assertIntegerType(toType, "toType");

    return createValueInstruction(InstructionOpcode::PtrToInt, toType, { value }, resultName);
}
Value* MirBuilder::createIntToPointer(Value* value, const MirType* toType, std::string_view resultName) {
    assertInteger(value, "value");
    assertPointerType(toType, "toType");

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
    
    const MirType* returnType = function->getType();
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
    const MirType* type = value->getType();
    VEE_ASSERT(type != nullptr, "{} is null!", name);
    VEE_ASSERT(type->isBool(), "{} must be of boolean type!", name);
}
void MirBuilder::assertInteger(Value* value, std::string_view name) {
    VEE_ASSERT(value != nullptr, "{} is null!", name);
    const MirType* type = value->getType();
    VEE_ASSERT(type != nullptr, "{} is null!", name);
    VEE_ASSERT(type->isInteger(), "{} must be of integer type!", name);
}
void MirBuilder::assertFloat(Value* value, std::string_view name) {
    VEE_ASSERT(value != nullptr, "{} is null!", name);
    const MirType* type = value->getType();
    VEE_ASSERT(type != nullptr, "{} is null!", name);
    VEE_ASSERT(type->isFloat(), "{} must be of float type!", name);
}
void MirBuilder::assertPointer(Value* value, std::string_view name) {
    VEE_ASSERT(value != nullptr, "{} is null!", name);
    const MirType* type = value->getType();
    VEE_ASSERT(type != nullptr, "{} is null!", name);
    VEE_ASSERT(type->isPointer(), "{} must be of pointer type!", name);
}

void MirBuilder::assertBooleanType(const MirType* type, std::string_view name) {
    VEE_ASSERT(type != nullptr, "{} is null!", name);
    VEE_ASSERT(type->isBool(), "{} must be of boolean type!", name);
}
void MirBuilder::assertIntegerType(const MirType* type, std::string_view name) {
    VEE_ASSERT(type != nullptr, "{} is null!", name);
    VEE_ASSERT(type->isInteger(), "{} must be of integer type!", name);
}
void MirBuilder::assertFloatType(const MirType* type, std::string_view name) {
    VEE_ASSERT(type != nullptr, "{} is null!", name);
    VEE_ASSERT(type->isFloat(), "{} must be of float type!", name);
}
void MirBuilder::assertPointerType(const MirType* type, std::string_view name) {
    VEE_ASSERT(type != nullptr, "{} is null!", name);
    VEE_ASSERT(type->isPointer(), "{} must be of pointer type!", name);
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
Value* MirBuilder::createValueInstruction(InstructionOpcode opcode, const MirType* type, basic::SmallVector<Value*, 2>&& operands, std::string_view resultName) {
    beginInstruction();
    Instruction* inst = _mir.factory.createInstruction(opcode, std::move(operands), type, resultName);
    finishInstruction(inst);
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

const MirType* MirBuilder::getBoolType() {
    return _mir.types.getBool();
}

} // namespace mir
VEEC_NAMESPACE_END
