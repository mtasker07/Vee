/*
* MemberAccessExprNode.hpp
*
* This file contains the definition of the MemberAccessExprNode AST node.
* The MemberAccessExprNode represents a member access expression in the program.
*
*/

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/source/Identifier.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/expr/ExpressionNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @enum MemberAccessOp
 * @brief Represents the different member access operators that can be used in a member access expression.
 */
enum class MemberAccessOp : u8 {
    Dot,
    Arrow,
};

/**
 * @class MemberAccessExprNode
 * @brief Member access expressions represent access to a member of an object.
 */
class MemberAccessExprNode : public ExpressionNode {
public:
    /**
     * @brief Constructs a new member access node with the given left-hand side and right-hand side.
     * @param lhs The left-hand side of this member access expression.
     * @param rhs The right-hand side of this member access expression.
     */
    MemberAccessExprNode(
        AstKey,
        ExpressionNode* object,
        source::Identifier member,
        MemberAccessOp op
    )
        : ExpressionNode(AstKey{}, AstKind::MemberAccessExpr),
        _lhs(object),
        _rhs(member),
        _op(op) {}

    virtual ~MemberAccessExprNode() = default;

    /**
     * @brief Gets the left-hand side of this member access expression (read-only).
     * @return The left-hand side of this member access expression.
     */
    inline const ExpressionNode* getObject() const { return _lhs; }
    /**
     * @brief Gets the left-hand side of this member access expression.
     * @return The left-hand side of this member access expression.
     */
    inline ExpressionNode* getObject() { return _lhs; }

    /**
     * @brief Gets the right-hand side of this member access expression (read-only).
     * @return The right-hand side of this member access expression.
     */
    inline const source::Identifier& getMember() const { return _rhs; }

    /**
     * @brief Gets the operator of this member access expression.
     * @return The operator of this member access expression.
     */
    inline MemberAccessOp getOperator() const { return _op; }

    /**
     * @brief Checks if the given AST node is a MemberAccessExprNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a MemberAccessExprNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::MemberAccessExpr;
    }

private:
    ExpressionNode* _lhs;
    source::Identifier _rhs;
    MemberAccessOp _op;
};

} // namespace ast
VEEC_NAMESPACE_END
