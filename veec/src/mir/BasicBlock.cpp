#include "veec/mir/BasicBlock.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/mir/Instruction.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

void BasicBlock::addInstruction(Instruction* instr) {
    VEE_ASSERT(instr != nullptr, "Instruction cannot be null");
    _instructions.push_back(instr);
}

} // namespace mir
VEEC_NAMESPACE_END
