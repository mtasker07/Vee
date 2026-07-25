/**
 * @file IndexExprNode.hpp
 * @brief This file contains the definition of the IndexExprNode AST node.
 * The IndexExprNode represents an array index expression in the program.
 *
 */

#pragma once

#include <vector>
#include <utility>

#include "vee/core/CoreDefines.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/expr/ExpressionNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @class IndexExprNode
 * @brief Index expressions represent array indexing operations.
 */
class IndexExprNode : public ExpressionNode {
public:
    /**
     * @brief Creates a new index expression node instance with the given object and indexer.
     * @param object The object expression of this index expression.
     * @param index The index expression of this index expression.
     */
    IndexExprNode(AstKey, ExpressionNode* object, ExpressionNode* index)
        : ExpressionNode(AstKey{}, AstKind::IndexExpr), _object(object), _index(index) {}

    virtual ~IndexExprNode() = default;

    /**
     * @brief Gets the object expression (typically an array) of this
     * index expression (read-only).
     * @return The object expression of this index expression.
     */
    inline const ExpressionNode* getObject() const { return _object; }
    /**
     * @brief Gets the object expression (typically an array) of this
     * index expression.
     * @return The object expression of this index expression.
     */
    inline ExpressionNode* getObject() { return _object; }

    /**
     * @brief Gets the index expression of this index expression (read-only).
     * @return The index expression of this index expression.
     */
    inline const ExpressionNode* getIndex() const { return _index; }
    /**
     * @brief Gets the index expression of this index expression.
     * @return The index expression of this index expression.
     */
    inline ExpressionNode* getIndex() { return _index; }

    /**
     * @brief Checks if the given AST node is an IndexExprNode.
     * @param node The AST node to check.
     * @return True if the given AST node is an IndexExprNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::IndexExpr;
    }

private:
    ExpressionNode* _object;
    ExpressionNode* _index;
};

} // namespace ast
VEEC_NAMESPACE_END
