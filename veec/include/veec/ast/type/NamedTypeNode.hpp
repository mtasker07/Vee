/**
 * @file NamedTypeNode.hpp
 * @brief This file contains the definition of the NamedTypeNode AST node.
 * The NamedType node represents a named type in the AST.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/basic/Token.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/type/TypeNode.hpp"
#include "veec/ast/name/QualifiedNameNode.hpp"
#include "veec/symbols/SymbolFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief This node represents a named type in the AST.
 */
class NamedTypeNode : public TypeNode {
public:
    /**
     * @brief Constructs a new named type node with the given qualified name.
     * @param qualifiedName The qualified name of this named type.
     */
    NamedTypeNode(AstKey, QualifiedNameNode* qualifiedName)
        : TypeNode(AstKey{}, AstKind::NamedType), _qualifiedName(qualifiedName) {}

    virtual ~NamedTypeNode() = default;

    /**
     * @brief Gets the resolved symbol of this named type, if any.
     * @return The resolved symbol of this named type, or nullptr if not resolved.
     * @note Shorthand for getQualifiedName()->resolvedSymbol.
     */
    inline symbols::Symbol* getResolvedSymbol() const {
        return _qualifiedName ? _qualifiedName->resolvedSymbol : nullptr;
    }

    /**
     * @brief Gets the qualified name of this named type (read-only).
     * @return The qualified name of this named type.
     */
    inline const QualifiedNameNode* getQualifiedName() const {
        return _qualifiedName;
    }
    /**
     * @brief Gets the qualified name of this named type.
     * @return The qualified name of this named type.
     */
    inline QualifiedNameNode* getQualifiedName() {
        return _qualifiedName;
    }

    /**
     * @brief Checks if the given AST node is a NamedTypeNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a NamedTypeNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::NamedType;
    }

private:
    QualifiedNameNode* _qualifiedName = nullptr;
};

} // namespace ast
VEEC_NAMESPACE_END
