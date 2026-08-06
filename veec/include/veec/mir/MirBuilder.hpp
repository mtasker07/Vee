/**
 * @file MirBuilder.hpp
 * @brief This file contains the definition of the MirBuilder class.
 * 
 * The MirBuilder class is responsible for constructing MIR.
 */

#pragma once

#include <type_traits>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/basic/APInt.hpp"
#include "veec/basic/SmallVector.hpp"
#include "veec/mir/MirContext.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/Instruction.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/types/TypeFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

/**
 * @class MirBuilder
 * @brief Responsible for constructing MIR.
 */
class MirBuilder {
public:
    /**
     * @brief Creates a new MirBuilder instance.
     * @param ctx The CompilationContext object.
     */
    MirBuilder(compilation::CompilationContext& ctx)
        : _ctx(ctx), _mir(ctx.mir) {}

    ~MirBuilder() = default;

    // Disallow copying and moving
    MirBuilder(const MirBuilder&) = delete;
    MirBuilder& operator=(const MirBuilder&) = delete;
    MirBuilder(MirBuilder&&) = delete;
    MirBuilder& operator=(MirBuilder&&) = delete;

    //
    // Context
    //

    /**
     * @brief Sets the insert block for this builder. The insert block is the block at which
     * created instructions will be inserted into. You can pass nullptr to clear the insert block,
     * or use clearInsertBlock().
     * @param block The block to set as the insert block.
     */
    inline void setInsertBlock(BasicBlock* block) {
        _insertBlock = block;
    }
    /**
     * @brief Clears the insert block for this builder. When the insert block is cleared, creating instructions
     * will throw a fatal error.
     * 
     * @note If you want to clear the insert block without asserting when adding instructions, it is recommended
     * to maintain a temporary discard block and set that as the insert block instead of clearing it.
     */
    inline void clearInsertBlock() {
        _insertBlock = nullptr;
    }
    /**
     * @brief Gets the current insert block for this builder.
     * @return The current insert block, or nullptr if no insert block is set.
     */
    inline BasicBlock* getInsertBlock() const {
        return _insertBlock;
    }

    //
    // Structural node creation
    //

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
    Function* createFunction(Module* module, symbols::FunctionSymbol* sym, std::string_view name = {});
    /**
     * @brief Creates a new basic block in the given function.
     * @param function The function to create the basic block in.
     * @param name The name of the basic block (optional).
     * @return A pointer to the new basic block.
     */
    BasicBlock* createBasicBlock(Function* function, std::string_view name = {});
    /**
     * @brief Creates a new local in the given function.
     * @param function The function to create the local in.
     * @param kind The kind of the local.
     * @param sym The symbol for the local.
     * @param name The name of the local (optional).
     * @return A pointer to the new local.
     */
    Local* createLocal(Function* function, LocalKind kind, symbols::VariableSymbol* sym, std::string_view name = {});

    /**
     * @brief Gets or creates a constant integer in the given module.
     * @param module The module to get or create the constant in.
     * @param type The type of the constant.
     * @param value The value of the constant.
     * @param name The name of the constant (optional).
     * @return A pointer to the constant integer.
     */
    ConstantInt* getConstantInt(Module* module, types::Type* type, basic::APInt value, std::string_view name = {});
    /**
     * @brief Gets or creates a constant integer with value 1 in the given module.
     * @param module The module to get or create the constant in.
     * @param type The type of the constant.
     * @param name The name of the constant (optional).
     * @return A pointer to the constant integer with value 1.
     */
    ConstantInt* getConstantIntOne(Module* module, types::Type* type, std::string_view name = {});
    /**
     * @brief Gets or creates a constant float in the given module.
     * @param module The module to get or create the constant in.
     * @param type The type of the constant.
     * @param value The value of the constant.
     * @param name The name of the constant (optional).
     * @return A pointer to the constant float.
     */
    ConstantFloat* getConstantFloat(Module* module, types::Type* type, double value, std::string_view name = {});
    /**
     * @brief Gets or creates a constant string in the given module.
     * @param module The module to get or create the constant in.
     * @param type The type of the constant.
     * @param value The value of the constant.
     * @param name The name of the constant (optional).
     * @return A pointer to the constant string.
     */
    ConstantString* getConstantString(Module* module, types::Type* type, std::string_view value, std::string_view name = {});
    /**
     * @brief Gets or creates a constant boolean in the given module.
     * @param module The module to get or create the constant in.
     * @param type The type of the constant.
     * @param value The value of the constant.
     * @param name The name of the constant (optional).
     * @return A pointer to the constant boolean.
     */
    ConstantBool* getConstantBool(Module* module, types::Type* type, bool value, std::string_view name = {});

    //
    // Basic
    //

    /**
     * @brief Creates a no-operation instruction.
     * This instruction does nothing and is used as a placeholder or for alignment purposes.
     */
    void createNop();

    //
    // SSA / value
    //

    // TODO
    Value* createPhi(
        types::Type* type,
        basic::SmallVector<std::pair<Value*, mir::BasicBlock*>>&& incomingValues
    );

    //
    // Integer arithmetic
    //

    /**
     * @brief Creates an integer negate instruction.
     * @param value The value to negate.
     * @param resultName The name of the result value (optional).
     * @return The negated value (integer).
     */
    Value* createNeg(Value* value, std::string_view resultName = {});
    /**
     * @brief Creates an integer addition instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (integer).
     */
    Value* createAdd(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates an integer subtraction instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (integer).
     */
    Value* createSub(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates an integer multiplication instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (integer).
     */
    Value* createMul(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a **signed** integer division instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (signed integer).
     */
    Value* createSDiv(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates an **unsigned** integer division instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (unsigned integer).
     */
    Value* createUDiv(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a **signed** integer modulo instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (signed integer).
     */
    Value* createSMod(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates an **unsigned** integer modulo instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (unsigned integer).
     */
    Value* createUMod(Value* lhs, Value* rhs, std::string_view resultName = {});

    //
    // Bitwise
    //

    Value* createBitNot(Value* value, std::string_view resultName = {});
    /**
     * @brief Creates an integer bitwise AND instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (integer).
     */
    Value* createBitAnd(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates an integer bitwise OR instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (integer).
     */
    Value* createBitOr(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates an integer bitwise XOR instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (integer).
     */
    Value* createBitXor(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates an integer bitwise left shift instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (integer).
     */
    Value* createBitShl(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates an **unsigned** integer bitwise logical right shift instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (unsigned integer).
     */
    Value* createBitLShr(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a **signed** integer bitwise arithmetic right shift instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (signed integer).
     */
    Value* createBitAShr(Value* lhs, Value* rhs, std::string_view resultName = {});

    //
    // Float arithmetic
    //

    /**
     * @brief Creates a float negate instruction.
     * @param value The value to negate.
     * @param resultName The name of the result value (optional).
     * @return The result value (float).
     */
    Value* createFNeg(Value* value, std::string_view resultName = {});
    /**
     * @brief Creates a float addition instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (float).
     */
    Value* createFAdd(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a float subtraction instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (float).
     */
    Value* createFSub(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a float multiplication instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (float).
     */
    Value* createFMul(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a float division instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (float).
     */
    Value* createFDiv(Value* lhs, Value* rhs, std::string_view resultName = {});

    //
    // Logical
    //

    /**
     * @brief Creates a logical NOT instruction.
     * @param value The value to logically negate.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createLogicalNot(Value* value, std::string_view resultName = {});
    /**
     * @brief Creates a logical AND instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createLogicalAnd(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a logical OR instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createLogicalOr(Value* lhs, Value* rhs, std::string_view resultName = {});

    //
    // Integer comparison
    //

    /**
     * @brief Creates an integer equality comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createICmpEq(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates an integer inequality comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createICmpNe(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a **signed** integer less-than comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createICmpSlt(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a **signed** integer less-than-or-equal comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createICmpSle(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a **signed** integer greater-than comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createICmpSgt(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a **signed** integer greater-than-or-equal comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createICmpSge(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates an **unsigned** integer less-than comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createICmpUlt(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates an **unsigned** integer less-than-or-equal comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createICmpUle(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates an **unsigned** integer greater-than comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createICmpUgt(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates an **unsigned** integer greater-than-or-equal comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createICmpUge(Value* lhs, Value* rhs, std::string_view resultName = {});

    //
    // Float comparison
    //

    /**
     * @brief Creates a float equality comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createFCmpEq(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a float inequality comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createFCmpNe(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a float less-than comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createFCmpLt(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a float less-than-or-equal comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createFCmpLe(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a float greater-than comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createFCmpGt(Value* lhs, Value* rhs, std::string_view resultName = {});
    /**
     * @brief Creates a float greater-than-or-equal comparison instruction.
     * @param lhs The left-hand side value.
     * @param rhs The right-hand side value.
     * @param resultName The name of the result value (optional).
     * @return The result value (boolean).
     */
    Value* createFCmpGe(Value* lhs, Value* rhs, std::string_view resultName = {});

    //
    // Memory & data
    //

    /**
     * @brief Creates a store instruction that stores the given value at the given address.
     * @param address The address to store the value at.
     * @param value The value to store.
     */
    void createStore(Value* address, Value* value);
    /**
     * @brief Creates a load instruction that loads a value from the given address.
     * @param address The address to load the value from.
     * @param resultName The name of the result value (optional).
     * @return The loaded value.
     */
    Value* createLoad(Value* address, std::string_view resultName = {});

    //
    // Data
    //

    /**
     * @brief Creates a construct instruction that constructs a value of the given type from the given values.
     * @param type The type of the value to construct.
     * @param values The values to construct the value from.
     * @param resultName The name of the result value (optional).
     * @return The constructed value.
     */
    Value* createConstruct(types::Type* type, basic::SmallVector<Value*, 2>&& values, std::string_view resultName = {});

    //
    // Conversion / casting
    //

    /**
     * @brief Creates a truncate instruction that truncates the given integer to the given type.
     * @param value The value to truncate.
     * @param toType The type to truncate the value to.
     * @param resultName The name of the result value (optional).
     * @return The truncated value.
     */
    Value* createTruncateInt(Value* value, types::Type* toType, std::string_view resultName = {});
    /**
     * @brief Creates an extend instruction that extends the given integer to the given type.
     * @param value The value to extend.
     * @param toType The type to extend the value to.
     * @param resultName The name of the result value (optional).
     * @return The extended value.
     */
    Value* createExtendInt(Value* value, types::Type* toType, std::string_view resultName = {});
    
    /**
     * @brief Creates a truncate instruction that truncates the given float to the given type.
     * @param value The value to truncate.
     * @param toType The type to truncate the value to.
     * @param resultName The name of the result value (optional).
     * @return The truncated value.
     */
    Value* createTruncateFloat(Value* value, types::Type* toType, std::string_view resultName = {});
    /**
     * @brief Creates an extend instruction that extends the given float to the given type.
     * @param value The value to extend.
     * @param toType The type to extend the value to.
     * @param resultName The name of the result value (optional).
     * @return The extended value.
     */
    Value* createExtendFloat(Value* value, types::Type* toType, std::string_view resultName = {});

    /**
     * @brief Creates an integer to float conversion instruction.
     * @param value The integer value to convert.
     * @param toType The float type to convert to.
     * @param resultName The name of the result value (optional).
     * @return The converted float value.
     */
    Value* createIntToFloat(Value* value, types::Type* toType, std::string_view resultName = {});
    /**
     * @brief Creates a float to integer conversion instruction.
     * @param value The float value to convert.
     * @param toType The integer type to convert to.
     * @param resultName The name of the result value (optional).
     * @return The converted integer value.
     */
    Value* createFloatToInt(Value* value, types::Type* toType, std::string_view resultName = {});

    /**
     * @brief Creates a pointer to integer conversion instruction.
     * @param value The pointer value to convert.
     * @param toType The integer type to convert to.
     * @param resultName The name of the result value (optional).
     * @return The converted integer value.
     */
    Value* createPointerToInt(Value* value, types::Type* toType, std::string_view resultName = {});
    /**
     * @brief Creates an integer to pointer conversion instruction.
     * @param value The integer value to convert.
     * @param toType The pointer type to convert to.
     * @param resultName The name of the result value (optional).
     * @return The converted pointer value.
     */
    Value* createIntToPointer(Value* value, types::Type* toType, std::string_view resultName = {});

    //
    // Control flow
    //

    Value* createCall(Function* function, const basic::SmallVector<Value*, 2>& args, std::string_view resultName = {});
    /**
     * @brief Creates a return instruction with the given return value.
     * @param returnValue The value to return.
     * @note To return void, use createRetVoid(), DON'T try to pass nullptr.
     */
    void createRet(Value* returnValue);
    /**
     * @brief Creates a return instruction with no return value (for void functions).
     */
    void createRetVoid();
    /**
     * @brief Creates an unconditional branch instruction to the given target block.
     * @param target The target basic block.
     */
    void createBranch(mir::BasicBlock* target);
    /**
     * @brief Creates a conditional branch instruction based on the given condition.
     * @param condition The condition value (must be a boolean).
     * @param trueTarget The target basic block if the condition is true.
     * @param falseTarget The target basic block if the condition is false.
     */
    void createConditionalBranch(Value* condition, mir::BasicBlock* trueTarget, mir::BasicBlock* falseTarget);
    /**
     * @brief Creates an unreachable instruction, indicating that execution flow will/should never reach
     * this point. This is mostly an optimization/validation hint, so it is not required, but it is highly
     * recommended for the best output.
     */
    void createUnreachable();

private:
    compilation::CompilationContext& _ctx;
    MirContext& _mir;

    BasicBlock* _insertBlock = nullptr;

    inline void insertGuard() {
        VEE_ASSERT(_insertBlock != nullptr, "No insert block set!");
    }

    //
    // Type checking (assertion)
    //

    void assertBoolean(Value* value, std::string_view name);
    void assertInteger(Value* value, std::string_view name);
    void assertSignedInteger(Value* value, std::string_view name);
    void assertUnsignedInteger(Value* value, std::string_view name);
    void assertFloat(Value* value, std::string_view name);
    void assertPointer(Value* value, std::string_view name);

    //
    // Instruction creation
    //

    void createInstruction(InstructionOpcode opcode);
    void createInstruction(InstructionOpcode opcode, basic::SmallVector<Value*, 2>&& operands);
    Value* createValueInstruction(InstructionOpcode opcode, types::Type* type, basic::SmallVector<Value*, 2>&& operands, std::string_view resultName);
    void beginInstruction();
    void finishInstruction(Instruction* instr);

    // Name given value
    void nameValue(const Value* val, std::string_view name);

    //
    // Type helpers
    //

    types::Type* getValueType(const Value* value);
    types::Type* getBoolType();
};

} // namespace mir
VEEC_NAMESPACE_END
