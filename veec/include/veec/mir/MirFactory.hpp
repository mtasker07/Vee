/**
 * @file MirFactory.hpp
 * @brief This file contains the definition of the MirFactory class.
 * 
 * The MirFactory class is responsible for creating MIR nodes.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/APInt.hpp"
#include "veec/basic/Arena.hpp"
#include "veec/basic/SmallVector.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/pretty/ValueNameMap.hpp"
#include "veec/mir/support/ValueTypeMap.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/types/TypeFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

enum class InstructionOpcode : u8;

/**
 * @class MirFactory
 * @brief Responsible for creating MIR nodes.
 */
class MirFactory {
public:
    /**
     * @brief Creates a new MirFactory instance.
     */
    MirFactory(
        basic::Arena<>& nodeArena,
        support::ValueTypeMap& valueTypes,
        pretty::ValueNameMap& valueNames
    )
        : _nodeArena(nodeArena),
        _valueTypes(valueTypes),
        _valueNames(valueNames) {}

    ~MirFactory() = default;

    // Disallow copying and moving
    MirFactory(const MirFactory&) = delete;
    MirFactory& operator=(const MirFactory&) = delete;
    MirFactory(MirFactory&&) = delete;
    MirFactory& operator=(MirFactory&&) = delete;

    /**
     * @brief Creates a new module.
     * @return A pointer to the new module.
     */
    Module* createModule();
    /**
     * @brief Creates a new function in the given module.
     * @param module The module to create the function in.
     * @param sym The function symbol for the function.
     * @param name The name of the function (optional).
     * @return A pointer to the new function.
     */
    Function* createFunction(
        Module* module,
        symbols::FunctionSymbol* sym,
        std::string_view name = {}
    );
    /**
     * @brief Creates a new basic block in the given function.
     * @param function The function to create the basic block in.
     * @param name The name of the basic block (optional).
     * @return A pointer to the new basic block.
     */
    BasicBlock* createBasicBlock(
        Function* function,
        std::string_view name = {}
    );
    /**
     * @brief Creates a new instruction.
     * @param opcode The opcode of the instruction.
     * @param resultType The type of the result value (optional).
     * @param resultName The name of the result value (optional).
     * @return A pointer to the new instruction.
     * @note HEAVILY not recommended to use this function. Use the MirBuilder instead,
     * which provides a much more convenient interface for creating instructions.
     */
    Instruction* createInstruction(
        InstructionOpcode opcode,
        types::Type* resultType = nullptr,
        std::string_view resultName = {}
    );
    /**
     * @brief Creates a new instruction.
     * @param opcode The opcode of the instruction.
     * @param operands The operands of the instruction.
     * @param resultType The type of the result value (optional).
     * @param resultName The name of the result value (optional).
     * @return A pointer to the new instruction.
     * @note HEAVILY not recommended to use this function. Use the MirBuilder instead,
     * which provides a much more convenient interface for creating instructions.
     */
    Instruction* createInstruction(
        InstructionOpcode opcode,
        basic::SmallVector<Value*, 2>&& operands,
        types::Type* resultType = nullptr,
        std::string_view resultName = {}
    );
    /**
     * @brief Creates a new local in the given function.
     * @param function The function to create the local in.
     * @param kind The kind of the local.
     * @param symbol The symbol for the local.
     * @param name The name of the local (optional).
     * @return A pointer to the new local.
     */
    Local* createLocal(
        Function* function,
        LocalKind kind,
        symbols::VariableSymbol* symbol,
        std::string_view name = {}
    );

    /**
     * @brief Gets or creates a constant integer in the given module.
     * @param module The module to get or create the constant in.
     * @param type The type of the constant.
     * @param value The value of the constant.
     * @param name The name of the constant (optional).
     * @return A pointer to the constant integer.
     */
    ConstantInt* getConstantInt(
        Module* module,
        types::Type* type,
        basic::APInt value,
        std::string_view name = {}
    );
    /**
     * @brief Gets or creates a constant integer with value 1 in the given module.
     * @param module The module to get or create the constant in.
     * @param type The type of the constant.
     * @param name The name of the constant (optional).
     * @return A pointer to the constant integer with value 1.
     */
    ConstantInt* getConstantIntOne(
        Module* module,
        types::Type* type,
        std::string_view name = {}
    );
    /**
     * @brief Gets or creates a constant float in the given module.
     * @param module The module to get or create the constant in.
     * @param type The type of the constant.
     * @param value The value of the constant.
     * @param name The name of the constant (optional).
     * @return A pointer to the constant float.
     */
    ConstantFloat* getConstantFloat(
        Module* module,
        types::Type* type,
        double value,
        std::string_view name = {}
    );
    /**
     * @brief Gets or creates a constant string in the given module.
     * @param module The module to get or create the constant in.
     * @param type The type of the constant.
     * @param value The value of the constant.
     * @param name The name of the constant (optional).
     * @return A pointer to the constant string.
     */
    ConstantString* getConstantString(
        Module* module,
        types::Type* type,
        std::string_view value,
        std::string_view name = {}
    );
    /**
     * @brief Gets or creates a constant boolean in the given module.
     * @param module The module to get or create the constant in.
     * @param type The type of the constant.
     * @param value The value of the constant.
     * @param name The name of the constant (optional).
     * @return A pointer to the constant boolean.
     */
    ConstantBool* getConstantBool(
        Module* module,
        types::Type* type,
        bool value,
        std::string_view name = {}
    );

private:
    basic::Arena<>& _nodeArena;
    support::ValueTypeMap& _valueTypes;
    pretty::ValueNameMap& _valueNames;

    template<typename T, typename... Args>
    inline T* makeNode(Args&&... args) {
        return _nodeArena.create<T>(typename MirNode::MirKey{}, std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    inline T* makeValue(types::Type* type, Args&&... args) {
        T* node = _nodeArena.create<T>(typename MirNode::MirKey{}, std::forward<Args>(args)...);
        if (type != nullptr) _valueTypes.setValueType(node, type);
        return node;
    }
    template<typename T, typename... Args>
    inline T* makeNamedValue(types::Type* type, std::string_view name, Args&&... args) {
        T* node = _nodeArena.create<T>(typename MirNode::MirKey{}, std::forward<Args>(args)...);
        if (type != nullptr) _valueTypes.setValueType(node, type);
        if (!name.empty()) _valueNames.setName(node, name);
        return node;
    }
};

} // namespace mir
VEEC_NAMESPACE_END
