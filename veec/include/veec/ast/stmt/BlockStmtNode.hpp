/**
 * @file BlockStmtNode.hpp
 * @brief This file contains the definition of the BlockStmtNode AST node.
 * The BlockStmt node represents a block of statements in the AST.
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
 * @brief This node represents a block of statements in the AST.
 */
class BlockStmtNode : public StatementNode {
public:
    BlockStmtNode(AstKey, std::vector<ItemNode*>&& items)
        : StatementNode(AstKey{}, AstKind::BlockStmt), _items(std::move(items)) {}

    virtual ~BlockStmtNode() = default;

    /**
     * @brief Gets the statements in this block statement (read-only).
     * @return The statements in this block statement.
     */
    inline const std::vector<ItemNode*>& getItems() const {
        return _items;
    }
    /**
     * @brief Gets the statements in this block statement.
     * @return The statements in this block statement.
     */
    inline std::vector<ItemNode*>& getItems() {
        return _items;
    }

    /**
     * @brief Checks if the given AST node is a BlockStmtNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a BlockStmtNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::BlockStmt;
    }

private:
    std::vector<ItemNode*> _items;
};

} // namespace ast
VEEC_NAMESPACE_END
