/**
 * @file ForStmtNode.hpp
 * @brief This file contains the definition of the ForStmtNode AST node.
 * The ForStmt node represents a for loop statement in the AST.
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
 * @brief This node represents a for loop statement in the AST.
 */
class ForStmtNode : public StatementNode {
public:
    /**
     * @brief Creates a new ForStmtNode instance.
	 * @param init The initializer statement for this for statement.
     * @param condition The condition expression for this for statement.
     * @param increment The increment expression for this for statement.
     * @param body The statement to execute in the for body.
     */
    ForStmtNode(
        AstKey,
        StatementNode* init,
		ExpressionNode* condition,
		ExpressionNode* increment,
		StatementNode* body
    )
        : StatementNode(AstKey{}, AstKind::ForStmt),
        _init(init),
        _condition(condition),
        _increment(increment),
        _body(body) {}

    virtual ~ForStmtNode() = default;

    /**
     * @brief Gets the initialization statement in this for loop statement.
     * @return The initialization statement in this for loop statement, or nullptr
     * if omitted.
     */
    inline const StatementNode* getInit() const {
        return _init;
    }
    /**
     * @brief Gets the initialization statement in this for loop statement.
     * @return The initialization statement in this for loop statement, or nullptr
     * if omitted.
     */
    inline StatementNode* getInit() {
        return _init;
    }
    /**
     * @brief Gets the condition expression in this for loop statement.
     * @return The condition expression in this for loop statement, or nullptr
     * if omitted (which is equivalent to true).
     */
    inline const ExpressionNode* getCondition() const {
        return _condition;
    }
    /**
     * @brief Gets the condition expression in this for loop statement.
     * @return The condition expression in this for loop statement, or nullptr
     * if omitted (which is equivalent to true).
     */
    inline ExpressionNode* getCondition() {
        return _condition;
    }
    /**
     * @brief Gets the increment expression in this for loop statement.
     * @return The increment expression in this for loop statement, or nullptr
     * if omitted.
     */
    inline const ExpressionNode* getIncrement() const {
        return _increment;
    }
    /**
     * @brief Gets the increment expression in this for loop statement.
     * @return The increment expression in this for loop statement, or nullptr
     * if omitted.
     */
    inline ExpressionNode* getIncrement() {
        return _increment;
    }

    /**
     * @brief Gets the body statement in this loop statement (read-only).
     * @return The body statement in this loop statement.
     */
    inline const StatementNode* getBody() const {
        return _body;
    }
    /**
     * @brief Gets the body statement in this loop statement.
     * @return The body statement in this loop statement.
     */
    inline StatementNode* getBody() {
        return _body;
    }

    /**
     * @brief Checks if the given AST node is a ForStmtNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a ForStmtNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::ForStmt;
    }

private:
    StatementNode* _init;
    ExpressionNode* _condition; // nullptr means true
    ExpressionNode* _increment;
    StatementNode* _body;
};

} // namespace ast
VEEC_NAMESPACE_END
