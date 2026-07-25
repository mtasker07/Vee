/**
 * @file ReturnStmtNode.hpp
 * @brief This file contains the definition of the ReturnStmtNode AST node.
 * The ReturnStmt node represents a return statement in the AST.
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
 * @brief This node represents a return statement in the AST.
 */
class ReturnStmtNode : public StatementNode {
public:
    ReturnStmtNode(AstKey, ExpressionNode* value)
        : StatementNode(AstKey{}, AstKind::ReturnStmt), _value(value) {}

    virtual ~ReturnStmtNode() = default;

    /**
     * @brief Gets the return value in this return statement (read-only).
     * @return The return value in this return statement, or nullptr if there is no return
     * value (i.e., a void return).
     */
    inline const ExpressionNode* getValue() const {
        return _value;
    }
    /**
     * @brief Gets the return value in this return statement.
     * @return The return value in this return statement, or nullptr if there is no return
     * value (i.e., a void return).
     */
	inline ExpressionNode* getValue() {
		return _value;
	}

    /**
     * @brief Checks if the given AST node is a ReturnStmtNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a ReturnStmtNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::ReturnStmt;
    }

private:
    ExpressionNode* _value;
};

} // namespace ast
VEEC_NAMESPACE_END
