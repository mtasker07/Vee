#include "veec/mir/Function.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/mir/BasicBlock.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

void Function::addBlock(BasicBlock& block) {
    if (block.getFunction() == this) {
        // Block already belongs to this function
        return;
    }

    VEE_ASSERT(block.getFunction() == nullptr,
        "Block already belongs to a different function!");

    _blocks.push_back(&block);
    block._function = this;
}

} // namespace mir
VEEC_NAMESPACE_END
