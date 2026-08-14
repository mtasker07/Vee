#include "veec/ast/AstNode.hpp"

#include <string>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/basic/StringPool.hpp"
#include "veec/io/IWriter.hpp"
#include "veec/io/StringWriter.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstPrinter.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

std::string AstNode::toString(const compilation::CompilationContext& ctx) const {
    AstPrinter printer(ctx);
    io::StringWriter writer;
    printer.printNode(*this, writer);
    return writer.str();
}

} // namespace ast
VEEC_NAMESPACE_END
