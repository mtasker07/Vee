/**
 * @file StatementNode.hpp
 * @brief This file contains the definition of the StatementNode AST node.
 * The Statement node represents a single statement in the AST. The statement
 * may either be a top-level statement or a simple statement within a block.
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
 * @brief This node represents a single statement in the AST.
 */
class StatementNode : public ItemNode {
public:
    virtual ~StatementNode() = default;

    /**
     * @brief Checks if the given AST node is a StatementNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a StatementNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        if (!node) return false;

        AstKind kind = node->getNodeKind();
        return
            kind >= AstKind::FirstStatement &&
            kind <= AstKind::LastStatement;
    }

protected:
    /**
     * @brief Creates an empty statement node with a given kind.
     * @param kind The kind of this statement node.
     */
    StatementNode(AstKey, AstKind kind)
        : ItemNode(AstKey{}, kind) {}
};

} // namespace ast
VEEC_NAMESPACE_END
