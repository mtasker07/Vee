/*
* NameExprNode.hpp
*
* This file contains the definition of the NameExprNode AST node.
* The NameExprNode represents a name expression in the program.
*
*/

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/source/Identifier.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/name/QualifiedNameNode.hpp"
#include "veec/ast/expr/ExpressionNode.hpp"
#include "veec/symbols/SymbolFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @class NameExprNode
 * @brief Name expressions represent a sequence of identifiers forming a name.
 */
class NameExprNode : public ExpressionNode {
public:
    /**
     * @brief Constructs a new name expression node with the given path of identifiers.
     * @param qualifiedName The qualified name ast node forming this name expression.
     */
    NameExprNode(AstKey, QualifiedNameNode* qualifiedName)
        : ExpressionNode(AstKey{}, AstKind::NameExpr), _qualifiedName(qualifiedName) {
        VEE_ASSERT(_qualifiedName, "NameExprNode must have a qualified name");
    }

    virtual ~NameExprNode() = default;

    /**
     * @brief Gets the qualified name forming this name expression (read-only).
     * @return The qualified name forming this name expression.
     */
    inline const QualifiedNameNode& getQualifiedName() const { return *_qualifiedName; }
    /**
     * @brief Gets the qualified name forming this name expression.
     * @return The qualified name forming this name expression.
     */
    inline QualifiedNameNode& getQualifiedName() { return *_qualifiedName; }

    /**
     * @brief Gets the resolved symbol for this name expression, if any (read-only).
     * This is basically just a shorthand for `getQualifiedName()->resolvedSymbol`.
     * @return The resolved symbol for this name expression, or nullptr if not resolved.
     */
    inline const symbols::Symbol* getResolvedSymbol() const {
        return _qualifiedName ? _qualifiedName->resolvedSymbol : nullptr;
    }
    /**
     * @brief Gets the resolved symbol for this name expression, if any (read-write).
     * This is basically just a shorthand for `getQualifiedName()->resolvedSymbol`.
     * @return The resolved symbol for this name expression, or nullptr if not resolved.
     */
    inline symbols::Symbol* getResolvedSymbol() {
        return _qualifiedName ? _qualifiedName->resolvedSymbol : nullptr;
    }

    /**
     * @brief Checks if the given AST node is a NameExprNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a NameExprNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::NameExpr;
    }

private:
    QualifiedNameNode* _qualifiedName = nullptr;
};

} // namespace ast
VEEC_NAMESPACE_END
