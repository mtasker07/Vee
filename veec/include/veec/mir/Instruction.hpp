/**
 * @file Instruction.hpp
 * @brief This file contains the definition of the Instruction class.
 * 
 * The Instruction class represents an instruction in the MIR. An instruction is a single operation
 * that can be carried out on the machine.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/MirKind.hpp"
#include "veec/mir/MirNode.hpp"
#include "veec/mir/Value.hpp"
#include "veec/types/TypeFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {
    
enum class InstructionOpcode : u8 {
    Nop, // No operation

    //
    // -- SSA / VALUE --
    //
    
    Phi, // Phi node

    //
    // -- ARITHMETIC --
    //

    // Integer
    Neg, // Integer negate
    Add, // Integer add
    Sub, // Integer subtract
    Mul, // Integer multiply
    SDiv, // Signed integer divide
    UDiv, // Unsigned integer divide
    SMod, // Signed integer modulo
    UMod, // Unsigned integer modulo

    // Integer (bitwise)
    BitNot, // Bitwise NOT
    BitAnd, // Bitwise AND
    BitOr, // Bitwise OR
    BitXor, // Bitwise XOR
    BitShl, // Bitwise shift left
    BitLShr, // Bitwise logical shift right
    BitAShr, // Bitwise arithmetic shift right

    // Floating
    FNeg, // Float negate
    FAdd, // Float add
    FSub, // Float subtract
    FMul, // Float multiply
    FDiv, // Float divide


    //
    // -- LOGICAL --
    //

    LogicalNot, // Logical NOT
    LogicalAnd, // Logical AND
    LogicalOr, // Logical OR


    //
    // -- COMPARISON --
    //

    // Integer
    ICmpEq, // Integer equal
    ICmpNe, // Integer not equal
    ICmpSlt, // Signed integer less than
    ICmpSle, // Signed integer less than or equal
    ICmpSgt, // Signed integer greater than
    ICmpSge, // Signed integer greater than or equal
    ICmpUlt, // Unsigned integer less than
    ICmpUle, // Unsigned integer less than or equal
    ICmpUgt, // Unsigned integer greater than
    ICmpUge, // Unsigned integer greater than or equal
    
    // Floating
    FCmpEq, // Float equal
    FCmpNe, // Float not equal
    FCmpLt, // Float less than
    FCmpLe, // Float less than or equal
    FCmpGt, // Float greater than
    FCmpGe, // Float greater than or equal

    
    //
    // -- MEMORY --
    //

    Store, // Store value at address
    Load, // Load value at address


    //
    // -- DATA --
    //

    Construct, // Construct composite type with values


    //
    // -- CONVERSION / CASTING --
    //

    // Integer
    TruncateInt, // Truncate integer to smaller size
    ZeroExtendInt, // Zero-extend integer to larger size
    SignExtendInt, // Sign-extend integer to larger size

    // Floating
    TruncateFloat, // Truncate float to smaller size
    ExtendFloat, // Extend float to larger size

    // Integer <-> Floating
    IntToFloat, // Convert signed integer to float
    UIntToFloat, // Convert unsigned integer to float
    FloatToInt, // Convert float to signed integer
    FloatToUInt, // Convert float to unsigned integer

    // Pointer
    PtrToInt, // Convert pointer to integer
    IntToPtr, // Convert integer to pointer
    
    // Reinterpretation
    Bitcast, // Reinterpret bits of value as another type



    //
    // -- CONTROL FLOW --
    //
    
    Call, // Call function
    Ret, // Return from function
    Br, // Unconditional branch
    CondBr, // Conditional branch
    Unreachable, // Unreachable instruction
};

/**
 * @brief Converts an InstructionOpcode to a human-readable string representation.
 * @param opcode The InstructionOpcode to convert.
 * @return A string representation of the InstructionOpcode.
 */
std::string_view toString(InstructionOpcode opcode);

/**
 * @class Instruction
 * @brief Represents an instruction in the MIR.
 * An instruction is a single operation that can be carried out on the machine.
 */
class Instruction : public User {
public:
    Instruction(
        MirKey key,
        InstructionOpcode opcode
    )
        : User(key, MirKind::Instruction),
        _opcode(opcode) {}
        
    Instruction(
        MirKey key,
        InstructionOpcode opcode,
        basic::SmallVector<Value*, 2>&& operands
    )
        : User(key, MirKind::Instruction, std::move(operands)),
        _opcode(opcode) {}

    ~Instruction() = default;

    /**
     * @brief Gets the opcode of this instruction.
     * @return The opcode of this instruction.
     */
    inline InstructionOpcode getOpcode() const { return _opcode; }

    /**
     * @brief Checks if this instruction produces a value.
     * @return True if this instruction produces a value, false otherwise.
     */
    inline bool producesValue() const {
        return _opcode != InstructionOpcode::Nop &&
               _opcode != InstructionOpcode::Ret &&
               _opcode != InstructionOpcode::Store &&
               _opcode != InstructionOpcode::Br &&
               _opcode != InstructionOpcode::CondBr &&
               _opcode != InstructionOpcode::Unreachable;
    }

private:
    friend class MirFactory;

    InstructionOpcode _opcode;
};

} // namespace mir
VEEC_NAMESPACE_END
