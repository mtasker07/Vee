#include "veec/ast/AstNode.hpp"

#include <string>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/CompilationContext.hpp"
#include "veec/basic/StringPool.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstPrinter.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

std::string AstNode::toString(const CompilationContext& ctx) const {
    AstPrinter printer(ctx);
    return printer.printNode(*this);
}

} // namespace ast
VEEC_NAMESPACE_END
