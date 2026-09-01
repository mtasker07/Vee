/**
 * @file BasicBlock.hpp
 * @brief This file contains the definition of the BasicBlock class.
 * 
 * The BasicBlock class represents a block in the MIR. A basic block is a sequence of instructions
 * that has a single entry point and a single exit point. It is a fundamental unit of control
 * flow in the MIR.
 * 
 * BasicBlock derives from Value to allow it to be used as an operand in instructions.
 */

#pragma once

#include <vector>
#include <string>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/Value.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

/**
 * @class BasicBlock
 * @brief Represents a basic block in the MIR.
 * A basic block is a sequence of instructions that has a single entry point and a single exit point.
 */
class BasicBlock : public Value {
public:
    BasicBlock(MirKey key)
        : Value(key, MirKind::BasicBlock) {}

    ~BasicBlock() = default;

    /**
     * @brief Gets the function to which this basic block belongs.
     * @return A pointer to the function that owns this basic block. nullptr
     * if the basic block is not owned by any function.
     */
    inline Function* getFunction() const { return _function; }

    /**
     * @brief Gets the instructions contained within this basic block.
     * @return A list of instructions contained within this basic block.
     */
    const std::vector<Instruction*>& getInstructions() const { return _instructions; }
    /**
     * @brief Gets the number of instructions in this basic block.
     * @return The number of instructions in this basic block.
     */
    inline size_t getInstructionCount() const { return _instructions.size(); }
    /**
     * @brief Gets the instruction at the given index in this basic block.
     * @param index The index of the instruction to get.
     * @return A pointer to the instruction at the given index.
     */
    inline Instruction* getInstruction(size_t index) const {
        VEE_ASSERT(index < _instructions.size(), "Index out of bounds");
        return _instructions[index];
    }
    /**
     * @brief Adds an instruction to this basic block. The instruction must not belong to
     * a basic block already.
     * @param instr The instruction to add.
     */
    void addInstruction(Instruction* instr);

private:
    friend class MirFactory;
    friend class Function;

    Function* _function = nullptr;
    std::vector<Instruction*> _instructions;
};

} // namespace mir
VEEC_NAMESPACE_END

