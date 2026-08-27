/**
 * @file BaseLoopStmtNode.hpp
 * @brief This file contains the definition of the BaseLoopStmtNode AST node.
 * The BaseLoopStmtNode node is the base class for all looping constructs and represents
 * any kind of loop statement in the AST.
 * 
 * This class is called BaseLoopStmtNode to avoid confusion with LoopStmtNode, which is
 * an actual language construct (a 'loop' statement) and not a base class.
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
 * @class BaseLoopStmtNode
 * @brief Base class for all loop constructs.
 */
class BaseLoopStmtNode : public StatementNode {
public:
    virtual ~BaseLoopStmtNode() = default;

    /**
     * @brief Checks if the given AST node is a BaseLoopStmtNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a BaseLoopStmtNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        if (!node) return false;
        
        AstKind kind = node->getNodeKind();
        return
            kind == AstKind::LoopStmt ||
            kind == AstKind::WhileStmt ||
            kind == AstKind::ForStmt;
    }

protected:
    /**
     * @brief Creates a new BaseLoopStmtNode with the given kind.
     * @param kind The specific kind of loop statement.
     */
    BaseLoopStmtNode(AstKey, AstKind kind)
        : StatementNode(AstKey{}, kind) {}
};

} // namespace ast
VEEC_NAMESPACE_END
