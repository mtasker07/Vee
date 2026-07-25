#include "veec/sema/ScopeManager.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema {

Scope* ScopeManager::createScope(Scope* parent) {
    Scope* newScope = _scopeArena.create<Scope>();
    newScope->_parent = parent;
    return newScope;
}

void ScopeManager::setNodeScope(ast::AstNode* node, Scope* scope) {
    _nodeToScope[node] = scope;
}
Scope* ScopeManager::getNodeScope(ast::AstNode* node) const {
    auto it = _nodeToScope.find(node);
    return it != _nodeToScope.end() ? it->second : nullptr;
}

} // namespace sema
VEEC_NAMESPACE_END
