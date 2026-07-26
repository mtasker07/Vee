/**
 * @file BinaryExprNode.hpp
 * @brief This file contains the definition of the BinaryExprNode AST node.
 * The BinaryExprNode represents a binary expression in the program.
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
 * @enum BinaryOp
 * @brief Represents the different binary operators that can be used in a binary expression.
 */
enum class BinaryOp : u8 {
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,
    Equal,
    NotEqual,
    LessThan,
    LessThanOrEqual,
    GreaterThan,
    GreaterThanOrEqual,
    LogicalAnd,
    LogicalOr,
};

/**
 * @brief Converts a BinaryOp to its string representation.
 * @param op The BinaryOp to convert.
 * @return A string representation of the BinaryOp.
 */
inline std::string_view toString(BinaryOp op) {
    // TODO: Use constants
    switch (op) {
        case BinaryOp::Add: return "+";
        case BinaryOp::Subtract: return "-";
        case BinaryOp::Multiply: return "*";
        case BinaryOp::Divide: return "/";
        case BinaryOp::Modulo: return "%";
        case BinaryOp::Equal: return "==";
        case BinaryOp::NotEqual: return "!=";
        case BinaryOp::LessThan: return "<";
        case BinaryOp::LessThanOrEqual: return "<=";
        case BinaryOp::GreaterThan: return ">";
        case BinaryOp::GreaterThanOrEqual: return ">=";
        case BinaryOp::LogicalAnd: return "&&";
        case BinaryOp::LogicalOr: return "||";
        default:
            VEE_UNREACHABLE("Unknown BinaryOp kind");
    }
}

/**
 * @class BinaryExprNode
 * @brief Represents a binary expression in the AST.
 * A binary expression consists of a left operand, a right operand, and an operator.
 */
class BinaryExprNode : public ExpressionNode {
public:
    /**
     * @brief Constructs a new binary expression node with the given operands and operator.
     * @param left The left operand of this binary expression.
     * @param right The right operand of this binary expression.
     * @param op The operator of this binary expression.
     */
    BinaryExprNode(AstKey, ExpressionNode* left, ExpressionNode* right, BinaryOp op)
        : ExpressionNode(AstKey{}, AstKind::BinaryExpr), _left(left), _right(right), _op(op) {}

    virtual ~BinaryExprNode() = default;

    /**
     * @brief Gets the left operand of this binary expression.
     * @return The left operand of this binary expression.
     */
    inline ExpressionNode* getLeft() { return _left; }
    /**
     * @brief Gets the left operand of this binary expression (immutable).
     * @return The left operand of this binary expression.
     */
    inline const ExpressionNode* getLeft() const { return _left; }
    /**
     * @brief Gets the right operand of this binary expression.
     * @return The right operand of this binary expression.
     */
    inline ExpressionNode* getRight() { return _right; }
    /**
     * @brief Gets the right operand of this binary expression (immutable).
     * @return The right operand of this binary expression.
     */
    inline const ExpressionNode* getRight() const { return _right; }
    /**
     * @brief Gets the operator of this binary expression.
     * @return The operator of this binary expression.
     */
    inline BinaryOp getOperator() const { return _op; }

    /**
     * @brief Checks if the given AST node is an BinaryExprNode.
     * @param node The AST node to check.
     * @return True if the given AST node is an BinaryExprNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::BinaryExpr;
    }

private:
    ExpressionNode* _left;
    ExpressionNode* _right;
    BinaryOp _op;
};

} // namespace ast
VEEC_NAMESPACE_END
