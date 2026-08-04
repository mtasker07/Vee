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
#include "veec/mir/BasicBlock.hpp"
#include "veec/mir/Instruction.hpp"
#include "veec/mir/Value.hpp"
#include "veec/mir/Constant.hpp"
#include "veec/symbols/ent/FunctionSymbol.hpp"
#include "veec/symbols/ent/VariableSymbol.hpp"
#include "veec/types/Type.hpp"
#include "veec/types/BuiltinType.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

Module* MirFactory::createModule() {
    return makeNode<Module>();
}

Function* MirFactory::createFunction(
    Module* module,
    symbols::FunctionSymbol* sym,
    std::string_view name
) {
    VEE_ASSERT(sym != nullptr, "Function symbol cannot be null");
    types::Type* returnType = sym->getReturnType();
    VEE_ASSERT(returnType != nullptr, "Return type cannot be null");

    auto func = makeNamedValue<Function>(returnType, name, sym);
    if (module) {
        module->addFunction(func);
    }
    return func;
}

BasicBlock* MirFactory::createBasicBlock(
    Function* function,
    std::string_view name
) {
    BasicBlock* block = makeNamedValue<BasicBlock>(nullptr, name);

    if (function) {
        function->addBlock(*block);
    }

    return block;
}

Instruction* MirFactory::createInstruction(
    InstructionOpcode opcode,
    types::Type* resultType,
    std::string_view resultName
) {
    return makeNamedValue<Instruction>(resultType, resultName, opcode);
}
Instruction* MirFactory::createInstruction(
    InstructionOpcode opcode,
    basic::SmallVector<Value*, 2>&& operands,
    types::Type* resultType,
    std::string_view resultName
) {
    return makeNamedValue<Instruction>(resultType, resultName, opcode, std::move(operands));
}

Local* MirFactory::createLocal(
    Function* function,
    LocalKind kind,
    symbols::VariableSymbol* symbol,
    std::string_view name
) {
    VEE_ASSERT(function != nullptr, "Function cannot be null");
    VEE_ASSERT(symbol != nullptr, "Variable symbol cannot be null");

    Local* local = makeNamedValue<Local>(symbol->getType(), name, function, kind, symbol);
    function->_locals.push_back(local);
    return local;
}

ConstantInt* MirFactory::getConstantInt(
    Module* module,
    types::Type* type,
    basic::APInt value,
    std::string_view name
) {
    VEE_ASSERT(module != nullptr, "Module cannot be null");
    VEE_ASSERT(type != nullptr, "Type cannot be null");
    VEE_ASSERT(type->is<types::BuiltinType>(), "Type must be a builtin type");
    VEE_ASSERT(type->as<types::BuiltinType>()->isInteger(), "Type must be an integer type");

    if (auto* existing = module->_constants.getInt(type, value)) {
        return existing;
    }

    ConstantInt* constant = makeNamedValue<ConstantInt>(type, name, value);
    module->_constants.intern(type, constant);
    return constant;
}

ConstantInt* MirFactory::getConstantIntOne(
    Module* module,
    types::Type* type,
    std::string_view name
) {
    VEE_ASSERT(module != nullptr, "Module cannot be null");
    VEE_ASSERT(type != nullptr, "Type cannot be null");
    VEE_ASSERT(type->is<types::BuiltinType>(), "Type must be a builtin type");
    VEE_ASSERT(type->as<types::BuiltinType>()->isInteger(), "Type must be an integer type");

    u32 bitWidth = type->as<types::BuiltinType>()->getBitWidth();
    basic::APInt oneValue = basic::APInt::fromU64(bitWidth, 1);

    if (auto* existing = module->_constants.getInt(type, oneValue)) {
        return existing;
    }

    ConstantInt* constant = makeNamedValue<ConstantInt>(type, name, oneValue);
    module->_constants.intern(type, constant);
    return constant;
}

ConstantFloat* MirFactory::getConstantFloat(
    Module* module,
    types::Type* type,
    double value,
    std::string_view name
) {
    VEE_ASSERT(module != nullptr, "Module cannot be null");
    VEE_ASSERT(type != nullptr, "Type cannot be null");
    VEE_ASSERT(type->is<types::BuiltinType>(), "Type must be a builtin type");
    VEE_ASSERT(type->as<types::BuiltinType>()->isFloatingPoint(), "Type must be a floating point type");

    if (auto* existing = module->_constants.getFloat(type, value)) {
        return existing;
    }

    ConstantFloat* constant = makeNamedValue<ConstantFloat>(type, name, value);
    module->_constants.intern(type, constant);
    return constant;
}

ConstantString* MirFactory::getConstantString(
    Module* module,
    types::Type* type,
    std::string_view value,
    std::string_view name
) {
    VEE_ASSERT(module != nullptr, "Module cannot be null");
    VEE_ASSERT(type != nullptr, "Type cannot be null");
    VEE_ASSERT(type->is<types::BuiltinType>(), "Type must be a builtin type");
    VEE_ASSERT(type->as<types::BuiltinType>()->isString(), "Type must be a string type");

    if (auto* existing = module->_constants.getString(type, std::string(value))) {
        return existing;
    }

    ConstantString* constant = makeNamedValue<ConstantString>(type, name, value);
    module->_constants.intern(type, constant);
    return constant;
}

ConstantBool* MirFactory::getConstantBool(
    Module* module,
    types::Type* type,
    bool value,
    std::string_view name
) {
    VEE_ASSERT(module != nullptr, "Module cannot be null");
    VEE_ASSERT(type != nullptr, "Type cannot be null");
    VEE_ASSERT(type->is<types::BuiltinType>(), "Type must be a builtin type");
    VEE_ASSERT(type->as<types::BuiltinType>()->isBoolean(), "Type must be a boolean type");

    if (auto* existing = module->_constants.getBool(value)) {
        return existing;
    }

    ConstantBool* constant = makeNamedValue<ConstantBool>(type, name, value);
    module->_constants.intern(constant);
    return constant;
}

} // namespace mir
VEEC_NAMESPACE_END
