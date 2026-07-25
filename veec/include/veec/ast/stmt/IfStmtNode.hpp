/**
 * @file IfStmtNode.hpp
 * @brief This file contains the definition of the IfStmtNode AST node.
 * The IfStmt node represents an if statement in the AST.
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
    * @brief This node represents an if statement in the AST.
    */
class IfStmtNode : public StatementNode {
public:
    /**
     * @brief Creates a new IfStmtNode with the given condition expression,
     * then statement, and optional else statement.
     * @param condition The condition expression for this if statement.
     * @param thenStmt The statement to execute if the condition is true.
     * @param elseStmt The statement to execute if the condition is false (optional).
     */
    IfStmtNode(
        AstKey,
        ExpressionNode* condition,
        StatementNode* thenStmt,
        StatementNode* elseStmt = nullptr
    )
        : StatementNode(AstKey{}, AstKind::IfStmt),
        _condition(condition),
        _thenStmt(thenStmt),
        _elseStmt(elseStmt) {}

    virtual ~IfStmtNode() = default;

    /**
     * @brief Gets the condition expression in this if statement (read-only).
     * @return The condition expression in this if statement.
     */
    inline const ExpressionNode* getCondition() const {
        return _condition;
    }
    /**
     * @brief Gets the condition expression in this if statement.
     * @return The condition expression in this if statement.
     */
	inline ExpressionNode* getCondition() {
		return _condition;
	}
    /**
     * @brief Gets the then statement in this if statement (read-only).
     * @return The then statement in this if statement.
     */
    inline const StatementNode* getThen() const {
        return _thenStmt;
    }
    /**
     * @brief Gets the then statement in this if statement.
     * @return The then statement in this if statement.
     */
    inline StatementNode* getThen() {
        return _thenStmt;
    }
    /**
     * @brief Gets the else statement in this if statement (read-only).
     * @return The else statement in this if statement, or nullptr if there is no else statement.
     */
    inline const StatementNode* getElse() const {
        return _elseStmt;
    }
    /**
     * @brief Gets the else statement in this if statement.
     * @return The else statement in this if statement, or nullptr if there is no else statement.
     */
    inline StatementNode* getElse() {
        return _elseStmt;
    }

    /**
     * @brief Checks if the given AST node is an IfStmtNode.
     * @param node The AST node to check.
     * @return True if the given AST node is an IfStmtNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::IfStmt;
    }

private:
    ExpressionNode* _condition;
    StatementNode* _thenStmt;
    StatementNode* _elseStmt;
};

} // namespace ast
VEEC_NAMESPACE_END
