/**
 * @file ExpressionNode.hpp
 * @brief This file contains the definition of the ExpressionNode AST node.
 * The Expression node represents a single expression in the program.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @class ExpressionNode
 * @brief Base class for all expressions.
 * Expressions are any constructs that can be evaluated to produce a value.
 */
class ExpressionNode : public AstNode {
public:
    virtual ~ExpressionNode() = default;

    /**
     * @brief Checks if the given AST node is an ExpressionNode.
     * @param node The AST node to check.
     * @return True if the given AST node is an ExpressionNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        if (!node) return false;

        AstKind kind = node->getNodeKind();
        return
            kind >= AstKind::FirstExpression &&
            kind <= AstKind::LastExpression;
    }

protected:
    /**
     * @brief Creates a new ExpressionNode with the given kind.
     * @param kind The kind of this ExpressionNode.
     */
    ExpressionNode(AstKey, AstKind kind)
        : AstNode(AstKey{}, kind) {}
};

} // namespace ast
VEEC_NAMESPACE_END
