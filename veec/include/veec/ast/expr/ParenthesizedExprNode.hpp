/**
 * @file ParenthesizedExprNode.hpp
 * @brief This file contains the definition of the ParenthesizedExprNode AST node.
 * The ParenthesizedExprNode represents a parenthesized expression in the program.
 *
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/expr/ExpressionNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @class ParenthesizedExprNode
 * @brief Parenthesized expressions represent any expression wrapped in parentheses.
 */
class ParenthesizedExprNode : public ExpressionNode {
public:
    /**
     * @brief Constructs a new parenthesized expression node with the given inner expression.
     * @param innerExpr The inner expression of this parenthesized expression.
     */
    ParenthesizedExprNode(AstKey, ExpressionNode* innerExpr)
        : ExpressionNode(AstKey{}, AstKind::ParenthesizedExpr), _innerExpr(innerExpr) {}

    virtual ~ParenthesizedExprNode() = default;

    /**
     * @brief Gets the inner expression of this parenthesized expression (read-only).
     * @return The inner expression of this parenthesized expression.
     */
    inline const ExpressionNode* getInnerExpr() const { return _innerExpr; }
    /**
     * @brief Gets the inner expression of this parenthesized expression.
     * @return The inner expression of this parenthesized expression.
     */
    inline ExpressionNode* getInnerExpr() { return _innerExpr; }

    /**
     * @brief Checks if the given AST node is a ParenthesizedExprNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a ParenthesizedExprNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::ParenthesizedExpr;
    }

private:
    ExpressionNode* _innerExpr;
};

} // namespace ast
VEEC_NAMESPACE_END
