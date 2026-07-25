/**
 * @file ScopeManager.hpp
 * @brief This file contains the definition of the ScopeManager class.
 * The ScopeManager class is responsible for managing scopes during semantic analysis.
 */

#pragma once

#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Arena.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/sema/Scope.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema {

/**
 * @class ScopeManager
 * @brief Responsible for managing scopes during semantic analysis.
 */
class ScopeManager {
public:
    /**
     * @brief Creates a new ScopeManager instance.
     */
    ScopeManager() = default;
    ~ScopeManager() = default;

    /**
     * @brief Creates a new scope and optionally sets its parent scope.
     * @param parent The parent scope of the new scope. If nullptr,
     * the new scope will be a top-level (global) scope.
     * @return A pointer to the newly created scope.
     */
    Scope* createScope(Scope* parent = nullptr);

    /**
     * @brief Associates a given AST node with a specific scope.
     * @param node The AST node to associate with the scope.
     * @param scope The scope to associate with the AST node.
     */
    void setNodeScope(ast::AstNode* node, Scope* scope);
    /**
     * @brief Gets the scope associated with a given AST node.
     * @param node The AST node for which to retrieve the associated scope.
     * @return A pointer to the associated scope, or nullptr if no scope is associated.
     */
    Scope* getNodeScope(ast::AstNode* node) const;

private:
    basic::Arena<> _scopeArena;
    std::unordered_map<ast::AstNode*, Scope*> _nodeToScope;
};

} // namespace sema
VEEC_NAMESPACE_END
