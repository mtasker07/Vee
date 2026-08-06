#include "veec/mir/MirNode.hpp"

#include <string>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/mir/pretty/MirPrinter.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

std::string MirNode::toString(const compilation::CompilationContext& ctx) const {
    pretty::MirPrinter printer(ctx);
    return printer.printNode(*this);
}

} // namespace mir
VEEC_NAMESPACE_END
