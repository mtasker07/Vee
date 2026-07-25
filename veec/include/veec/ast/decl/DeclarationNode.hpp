/**
 * @file DeclarationNode.hpp
 * @brief This file contains the definition of the DeclarationNode AST node.
 * The Declaration node represents a single declaration in the AST.
 */

#pragma once

#include <vector>
#include <memory>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/ItemNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief This node represents a single declaration in the AST.
 */
class DeclarationNode : public ItemNode {
public:
    virtual ~DeclarationNode() = default;

    /**
     * @brief Checks if the given AST node is a DeclarationNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a DeclarationNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        if (!node) return false;

        AstKind kind = node->getNodeKind();
        return
            kind >= AstKind::FirstDeclaration &&
            kind <= AstKind::LastDeclaration;
    }

protected:
    /**
     * @brief Creates an empty declaration node with a given kind.
     * @param kind The kind of this declaration node.
     */
    DeclarationNode(AstKey, AstKind kind)
        : ItemNode(AstKey{}, kind) {}
};

} // namespace ast
VEEC_NAMESPACE_END
