#include "veec/mir/MirFactory.hpp"

#include <string>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/basic/APInt.hpp"
#include "veec/basic/SmallVector.hpp"
#include "veec/mir/Module.hpp"
#include "veec/mir/Function.hpp"
#include "veec/mir/Argument.hpp"
#include "veec/mir/BasicBlock.hpp"
#include "veec/mir/Instruction.hpp"
#include "veec/mir/Value.hpp"
#include "veec/mir/Constant.hpp"
#include "veec/mir/MirType.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

Module* MirFactory::createModule() {
    return makeNode<Module>();
}
Function* MirFactory::createFunction(
    Module* module,
    const MirFunctionType* type,
    std::string_view name
) {
    VEE_ASSERT(type != nullptr, "Function type cannot be null");

    auto func = makeNamedValue<Function>(name, type);
    if (module) {
        module->addFunction(func);
    }
    return func;
}
Argument* MirFactory::createArgument(
    Function* function,
    const MirType* type,
    std::string_view name
) {
    VEE_ASSERT(function != nullptr, "Function cannot be null");
    VEE_ASSERT(type != nullptr, "Argument type cannot be null");

    Argument* arg = makeNamedValue<Argument>(name, type, function);
    function->_args.push_back(arg);
    return arg;
}
BasicBlock* MirFactory::createBasicBlock(
    Function* function,
    std::string_view name
) {
    BasicBlock* block = makeNamedValue<BasicBlock>(name);

    if (function) {
        function->addBlock(*block);
    }

    return block;
}
Instruction* MirFactory::createInstruction(
    InstructionOpcode opcode,
    const MirType* resultType,
    std::string_view resultName
) {
    return makeNamedValue<Instruction>(resultName, opcode, resultType);
}
Instruction* MirFactory::createInstruction(
    InstructionOpcode opcode,
    basic::SmallVector<Value*, 2>&& operands,
    const MirType* resultType,
    std::string_view resultName
) {
    return makeNamedValue<Instruction>(resultName, opcode, std::move(operands), resultType);
}

ConstantInt* MirFactory::getConstantInt(
    Module* module,
    const MirType* type,
    basic::APInt value,
    std::string_view name
) {
    VEE_ASSERT(module != nullptr, "Module cannot be null");
    VEE_ASSERT(type != nullptr, "Type cannot be null");
    VEE_ASSERT(type->isInteger(), "Type must be an integer type");

    if (auto* existing = module->_constants.getInt(type, value)) {
        return existing;
    }

    ConstantInt* constant = makeNamedValue<ConstantInt>(name, value, type);
    module->_constants.intern(type, constant);
    return constant;
}
ConstantInt* MirFactory::getConstantIntOne(
    Module* module,
    const MirType* type,
    std::string_view name
) {
    VEE_ASSERT(module != nullptr, "Module cannot be null");
    VEE_ASSERT(type != nullptr, "Type cannot be null");
    VEE_ASSERT(type->isInteger(), "Type must be an integer type");

    u32 bitWidth = type->getBitWidth();
    basic::APInt oneValue = basic::APInt::fromU64(bitWidth, 1);

    if (auto* existing = module->_constants.getInt(type, oneValue)) {
        return existing;
    }

    ConstantInt* constant = makeNamedValue<ConstantInt>(name, oneValue, type);
    module->_constants.intern(type, constant);
    return constant;
}
ConstantFloat* MirFactory::getConstantFloat(
    Module* module,
    const MirType* type,
    double value,
    std::string_view name
) {
    VEE_ASSERT(module != nullptr, "Module cannot be null");
    VEE_ASSERT(type != nullptr, "Type cannot be null");
    VEE_ASSERT(type->isFloat(), "Type must be a floating point type");

    if (auto* existing = module->_constants.getFloat(type, value)) {
        return existing;
    }

    ConstantFloat* constant = makeNamedValue<ConstantFloat>(name, value, type);
    module->_constants.intern(type, constant);
    return constant;
}
ConstantString* MirFactory::getConstantString(
    Module* module,
    const MirType* type,
    std::string_view value,
    std::string_view name
) {
    VEE_ASSERT(module != nullptr, "Module cannot be null");
    VEE_ASSERT(type != nullptr, "Type cannot be null");
    VEE_ASSERT(type->isStruct(), "Type must be a string struct type");

    if (auto* existing = module->_constants.getString(type, std::string(value))) {
        return existing;
    }

    ConstantString* constant = makeNamedValue<ConstantString>(name, value, type);
    module->_constants.intern(type, constant);
    return constant;
}
ConstantBool* MirFactory::getConstantBool(
    Module* module,
    const MirType* type,
    bool value,
    std::string_view name
) {
    VEE_ASSERT(module != nullptr, "Module cannot be null");
    VEE_ASSERT(type != nullptr, "Type cannot be null");
    VEE_ASSERT(type->isBool(), "Type must be a boolean type");

    if (auto* existing = module->_constants.getBool(value)) {
        return existing;
    }

    ConstantBool* constant = makeNamedValue<ConstantBool>(name, value, type);
    module->_constants.intern(constant);
    return constant;
}

} // namespace mir
VEEC_NAMESPACE_END
