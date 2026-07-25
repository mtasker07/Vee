/**
 * @file LoopStmtNode.hpp
 * @brief This file contains the definition of the LoopStmtNode AST node.
 * The LoopStmt node represents a loop statement in the AST.
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
 * @brief This node represents a loop statement in the AST.
 */
class LoopStmtNode : public StatementNode {
public:
    /**
     * @brief Creates a new LoopStmtNode with the given condition expression and body statement.
     * @param condition The condition expression for this loop statement.
     * @param body The statement to execute in the loop body.
     */
    LoopStmtNode(AstKey, StatementNode* body)
        : StatementNode(AstKey{}, AstKind::LoopStmt), _body(body) {}

    virtual ~LoopStmtNode() = default;

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
     * @brief Checks if the given AST node is a LoopStmtNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a LoopStmtNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::LoopStmt;
    }

private:
    StatementNode* _body;
};

} // namespace ast
VEEC_NAMESPACE_END
