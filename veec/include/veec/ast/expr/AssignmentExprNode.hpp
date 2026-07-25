/**
 * @file AssignmentExprNode.hpp
 * @brief This file contains the definition of the AssignmentExprNode AST node.
 * The AssignmentExprNode represents an assignment expression in the program.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/expr/ExpressionNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @enum AssignmentOp
 * @brief Represents the different assignment operators that can be used in an assignment expression.
 */
enum class AssignmentOp : u8 {
    Assign,
    AddAssign,
    SubtractAssign,
    MultiplyAssign,
    DivideAssign,
    ModuloAssign,
};

/**
 * @class AssignmentExprNode
 * @brief Represents an assignment expression in the AST.
 * An assignment expression consists of a left operand, a right operand, and an assignment operator.
 */
class AssignmentExprNode : public ExpressionNode {
public:
    /**
     * @brief Constructs a new assignment expression node with the given operands and operator.
     * @param left The left operand of this assignment expression.
     * @param right The right operand of this assignment expression.
     * @param op The operator of this assignment expression.
     */
    AssignmentExprNode(
        AstKey,
        ExpressionNode* left,
        ExpressionNode* right,
        AssignmentOp op
    )
        : ExpressionNode(AstKey{}, AstKind::AssignmentExpr),
        _left(left),
        _right(right),
        _op(op) {}

    virtual ~AssignmentExprNode() = default;

    /**
     * @brief Gets the left operand of this assignment expression (read-only).
     * @return The left operand of this assignment expression.
     */
    inline const ExpressionNode* getLeft() const { return _left; }
    /**
     * @brief Gets the left operand of this assignment expression.
     * @return The left operand of this assignment expression.
     */
    inline ExpressionNode* getLeft() { return _left; }

    /**
     * @brief Gets the right operand of this assignment expression (read-only).
     * @return The right operand of this assignment expression.
     */
    inline const ExpressionNode* getRight() const { return _right; }
    /**
     * @brief Gets the right operand of this assignment expression.
     * @return The right operand of this assignment expression.
     */
    inline ExpressionNode* getRight() { return _right; }
    
    /**
     * @brief Gets the operator of this assignment expression.
     * @return The operator of this assignment expression.
     */
    inline AssignmentOp getOperator() const { return _op; }

    /**
     * @brief Checks if the given AST node is an AssignmentExprNode.
     * @param node The AST node to check.
     * @return True if the given AST node is an AssignmentExprNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::AssignmentExpr;
    }

private:
    ExpressionNode* _left;
    ExpressionNode* _right;
    AssignmentOp _op;
};

} // namespace ast
VEEC_NAMESPACE_END
