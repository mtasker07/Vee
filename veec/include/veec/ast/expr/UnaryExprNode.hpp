/**
 * @file UnaryExprNode.hpp
 * @brief This file contains the definition of the UnaryExprNode AST node.
 * The UnaryExprNode represents a unary expression in the program.
 *
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/expr/ExpressionNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @enum UnaryOp
 * @brief Represents the different kinds of unary operators in the language.
 */
enum class UnaryOp : u8 {
    Plus,
    Minus,
    Increment, // Pre increment
    Decrement, // Pre decrement
    LogicalNot,
    BitwiseNot,
    Dereference,
    AddressOf
};

/**
 * @brief Converts a UnaryOp to its string representation.
 * @param op The UnaryOp to convert.
 * @return A string representation of the UnaryOp.
 */
inline std::string_view toString(UnaryOp op) {
    // TODO: Use constants
    switch (op) {
        case UnaryOp::Plus: return "+";
        case UnaryOp::Minus: return "-";
        case UnaryOp::Increment: return "++";
        case UnaryOp::Decrement: return "--";
        case UnaryOp::LogicalNot: return "!";
        case UnaryOp::BitwiseNot: return "~";
        case UnaryOp::Dereference: return "*";
        case UnaryOp::AddressOf: return "&";
        default:
            VEE_UNREACHABLE("Unknown UnaryOp kind");
    }
}

/**
 * @class UnaryExprNode
 * @brief Unary expressions represent any unary operation in the program, e.g. negation.
 */
class UnaryExprNode : public ExpressionNode {
public:
    /**
     * @brief Constructs a new unary expression node with the given operand and operator.
     * @param operand The operand of this unary expression.
     * @param op The operator of this unary expression.
     */
    UnaryExprNode(AstKey, ExpressionNode* operand, UnaryOp op)
    : ExpressionNode(AstKey{}, AstKind::UnaryExpr),
                _operand(operand),
        _op(op) {}

    virtual ~UnaryExprNode() = default;

    /**
     * @brief Gets the operand of this unary expression (read-only).
     * @return The operand of this unary expression.
     */
    inline const ExpressionNode* getOperand() const { return _operand; }
    /**
     * @brief Gets the operand of this unary expression.
     * @return The operand of this unary expression.
     */
    inline ExpressionNode* getOperand() { return _operand; }

    /**
     * @brief Gets the operator of this unary expression.
     * @return The operator of this unary expression.
     */
    inline UnaryOp getOperator() const { return _op; }

    /**
     * @brief Checks if the given AST node is a UnaryExprNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a UnaryExprNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::UnaryExpr;
    }

private:
    ExpressionNode* _operand;
    UnaryOp _op;
};

} // namespace ast
VEEC_NAMESPACE_END
