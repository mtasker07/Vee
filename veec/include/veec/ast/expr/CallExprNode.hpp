/**
 * @file CallExprNode.hpp
 * @brief This file contains the definition of the CallExprNode AST node.
 * The CallExprNode represents a function/method call expression in the program.
 *
 */

#pragma once

#include <vector>
#include <utility>

#include "vee/core/CoreDefines.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/expr/ExpressionNode.hpp"
#include "veec/symbols/SymbolFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @class CallExprNode
 * @brief Call expressions represent function or method calls.
 */
class CallExprNode : public ExpressionNode {
public:
    symbols::FunctionSymbol* symbol = nullptr;

    /**
     * @brief Creates a new call expression node instance with the given callee and arguments.
     * @param callee The callee expression of this call expression.
     * @param args The arguments of this call expression.
     */
    CallExprNode(AstKey, ExpressionNode* callee, std::vector<ExpressionNode*> args)
        : ExpressionNode(AstKey{}, AstKind::CallExpr), _callee(callee), _args(std::move(args)) {}

    virtual ~CallExprNode() = default;

    /**
     * @brief Gets the callee expression of this call expression (read-only).
     * @return The callee expression of this call expression.
     */
    inline const ExpressionNode* getCallee() const { return _callee; }
    /**
     * @brief Gets the callee expression of this call expression.
     * @return The callee expression of this call expression.
     */
    inline ExpressionNode* getCallee() { return _callee; }

    /**
     * @brief Gets the arguments of this call expression (read-only).
     * @return The arguments of this call expression.
     */
    inline const std::vector<ExpressionNode*>& getArgs() const { return _args; }
    /**
     * @brief Gets the arguments of this call expression.
     * @return The arguments of this call expression.
     */
    inline std::vector<ExpressionNode*>& getArgs() { return _args; }

    /**
     * @brief Checks if the given AST node is a CallExprNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a CallExprNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::CallExpr;
    }

private:
    ExpressionNode* _callee;
    std::vector<ExpressionNode*> _args;
};

} // namespace ast
VEEC_NAMESPACE_END
