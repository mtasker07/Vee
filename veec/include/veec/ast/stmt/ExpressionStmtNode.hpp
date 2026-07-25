/**
 * @file ExpressionStmtNode.hpp
 * @brief This file contains the definition of the ExpressionStmtNode AST node.
 * The ExpressionStmt node represents an expression statement in the AST.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/basic/Token.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/stmt/StatementNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief This node represents an expression statement in the AST.
 */
class ExpressionStmtNode : public StatementNode {
public:
    ExpressionStmtNode(AstKey, ExpressionNode* expression)
        : StatementNode(AstKey{}, AstKind::ExpressionStmt), _expression(expression) {}

    virtual ~ExpressionStmtNode() = default;

    /**
     * @brief Gets the expression in this expression statement (read-only).
     * @return The expression in this expression statement.
     */
    inline const ExpressionNode* getExpression() const {
        return _expression;
    }
    /**
     * @brief Gets the expression in this expression statement.
     * @return The expression in this expression statement.
     */
	inline ExpressionNode* getExpression() {
		return _expression;
	}

    /**
     * @brief Checks if the given AST node is an ExpressionStmtNode.
     * @param node The AST node to check.
     * @return True if the given AST node is an ExpressionStmtNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::ExpressionStmt;
    }

private:
    ExpressionNode* _expression;
};

} // namespace ast
VEEC_NAMESPACE_END
