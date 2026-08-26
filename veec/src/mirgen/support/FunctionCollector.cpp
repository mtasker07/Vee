#include "veec/mirgen/support/FunctionCollector.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/decl/FunctionDeclNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace mirgen {
namespace support {

std::vector<const ast::FunctionDeclNode*> FunctionCollector::collectFunctions(const ast::AstNode& node) {
    _functions.clear();
    walk(node);
    return _functions;
}
    
void FunctionCollector::visitFunctionDecl(const ast::FunctionDeclNode& node) {
    _functions.push_back(&node);
}

} // namespace support
} // namespace mirgen
VEEC_NAMESPACE_END
