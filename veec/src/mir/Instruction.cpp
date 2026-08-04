#include "veec/mir/Instruction.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/mir/MirFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

std::string_view toString(InstructionOpcode opcode) {
    switch (opcode) {
        case InstructionOpcode::Nop: return "nop";

        // SSA / Value
        case InstructionOpcode::Phi: return "phi";

        // Arithmetic - Integer
        case InstructionOpcode::Neg: return "neg";
        case InstructionOpcode::Add: return "add";
        case InstructionOpcode::Sub: return "sub";
        case InstructionOpcode::Mul: return "mul";
        case InstructionOpcode::SDiv: return "sdiv";
        case InstructionOpcode::UDiv: return "udiv";
        case InstructionOpcode::SMod: return "smod";
        case InstructionOpcode::UMod: return "umod";

        // Arithmetic - Bitwise
        case InstructionOpcode::BitNot: return "bitnot";
        case InstructionOpcode::BitAnd: return "bitand";
        case InstructionOpcode::BitOr: return "bitor";
        case InstructionOpcode::BitXor: return "bitxor";
        case InstructionOpcode::BitShl: return "bitshl";
        case InstructionOpcode::BitLShr: return "bitlshr";
        case InstructionOpcode::BitAShr: return "bitashr";

        // Arithmetic - Floating
        case InstructionOpcode::FNeg: return "fneg";
        case InstructionOpcode::FAdd: return "fadd";
        case InstructionOpcode::FSub: return "fsub";
        case InstructionOpcode::FMul: return "fmul";
        case InstructionOpcode::FDiv: return "fdiv";

        // Logical
        case InstructionOpcode::LogicalNot: return "lnot";
        case InstructionOpcode::LogicalAnd: return "land";
        case InstructionOpcode::LogicalOr: return "lor";

        // Comparison - Integer
        case InstructionOpcode::ICmpEq: return "icmp.eq";
        case InstructionOpcode::ICmpNe: return "icmp.ne";
        case InstructionOpcode::ICmpSlt: return "icmp.slt";
        case InstructionOpcode::ICmpSle: return "icmp.sle";
        case InstructionOpcode::ICmpSgt: return "icmp.sgt";
        case InstructionOpcode::ICmpSge: return "icmp.sge";
        case InstructionOpcode::ICmpUlt: return "icmp.ult";
        case InstructionOpcode::ICmpUle: return "icmp.ule";
        case InstructionOpcode::ICmpUgt: return "icmp.ugt";
        case InstructionOpcode::ICmpUge: return "icmp.uge";

        // Comparison - Floating
        case InstructionOpcode::FCmpEq: return "fcmp.eq";
        case InstructionOpcode::FCmpNe: return "fcmp.ne";
        case InstructionOpcode::FCmpLt: return "fcmp.lt";
        case InstructionOpcode::FCmpLe: return "fcmp.le";
        case InstructionOpcode::FCmpGt: return "fcmp.gt";
        case InstructionOpcode::FCmpGe: return "fcmp.ge";

        // Memory
        case InstructionOpcode::Store: return "store";
        case InstructionOpcode::Load: return "load";

        // Data
        case InstructionOpcode::Construct: return "construct";

        // Conversion / Casting
        case InstructionOpcode::TruncateInt: return "truncint";
        case InstructionOpcode::ZeroExtendInt: return "zextint";
        case InstructionOpcode::SignExtendInt: return "sextint";
        case InstructionOpcode::TruncateFloat: return "truncfloat";
        case InstructionOpcode::ExtendFloat: return "extfloat";
        case InstructionOpcode::IntToFloat: return "inttofloat";
        case InstructionOpcode::UIntToFloat: return "uinttofloat";
        case InstructionOpcode::FloatToInt: return "floattoint";
        case InstructionOpcode::FloatToUInt: return "floattouint";
        case InstructionOpcode::PtrToInt: return "ptrtoint";
        case InstructionOpcode::IntToPtr: return "inttoptr";
        case InstructionOpcode::Bitcast: return "bitcast";

        // Control flow
        case InstructionOpcode::Call: return "call";
        case InstructionOpcode::Ret: return "ret";
        case InstructionOpcode::Br: return "br";
        case InstructionOpcode::CondBr: return "condbr";
        case InstructionOpcode::Unreachable: return "unreachable";

        default:
            VEE_UNREACHABLE("Unknown InstructionOpcode");
    }
}

} // namespace mir
VEEC_NAMESPACE_END
