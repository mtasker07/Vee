/**
 * @file ConstructExprNode.hpp
 * @brief This file contains the definition of the ConstructExprNode AST node.
 * The ConstructExprNode represents an object construction "literal" in the program.
 *
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/expr/ExpressionNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @class ConstructExprNode
 * @brief Construct expressions represent object construction "literals" in the program.
 */
class ConstructExprNode : public ExpressionNode {
public:
    /**
     * @brief Constructs a new construct expression node with the given arguments.
     * @param args The arguments of this construct expression.
     */
    ConstructExprNode(AstKey, TypeNode* type, std::vector<ExpressionNode*> args)
        : ExpressionNode(AstKey{}, AstKind::ConstructExpr), _type(type), _args(std::move(args)) {}

    virtual ~ConstructExprNode() = default;

    /**
     * @brief Gets the type of this construct expression (read-only).
     * @return The type of this construct expression.
     */
    inline const TypeNode* getType() const { return _type; }
    /**
     * @brief Gets the type of this construct expression.
     * @return The type of this construct expression.
     */
    inline TypeNode* getType() { return _type; }
    
    /**
     * @brief Gets the arguments of this construct expression (read-only).
     * @return The arguments of this construct expression.
     */
    inline const std::vector<ExpressionNode*>& getArgs() const { return _args; }
    /**
     * @brief Gets the arguments of this construct expression.
     * @return The arguments of this construct expression.
     */
    inline std::vector<ExpressionNode*>& getArgs() { return _args; }

    /**
     * @brief Checks if the given AST node is a ConstructExprNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a ConstructExprNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::ConstructExpr;
    }

private:
    TypeNode* _type = nullptr;
    // TODO: Make this a dedicated node for designators.
    std::vector<ExpressionNode*> _args;
};

} // namespace ast
VEEC_NAMESPACE_END
