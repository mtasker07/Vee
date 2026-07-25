/**
 * @file ItemNode.hpp
 * @brief This file contains the definition of the ItemNode AST node.
 * The Item node represents a single (scoped) item in the AST. This is
 * usually a declaration or top-level statement.
 */

#pragma once

#include <vector>
#include <memory>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief This node represents a single (scoped) item in the AST.
 * This is usually a declaration or top-level statement.
 */
class ItemNode : public AstNode {
public:
    virtual ~ItemNode() = default;

    /**
     * @brief Checks if the given AST node is an ItemNode.
     * @param node The AST node to check.
     * @return True if the given AST node is an ItemNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        if (!node) return false;

        AstKind kind = node->getNodeKind();
        return
            kind >= AstKind::FirstDeclaration &&
            kind <= AstKind::LastDeclaration ||
            kind >= AstKind::FirstStatement &&
            kind <= AstKind::LastStatement;
    }

protected:
    /**
     * @brief Creates an empty item node with a given kind.
     * @param kind The kind of this item node.
     */
    ItemNode(AstKey, AstKind kind)
        : AstNode(AstKey{}, kind) {}
};

} // namespace ast
VEEC_NAMESPACE_END
