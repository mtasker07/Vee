/**
 * @file ParameterDeclNode.hpp
 * @brief This file contains the definition of the ParameterDeclNode AST node.
 * The ParameterDecl node represents a single parameter declaration in the AST.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/basic/Token.hpp"
#include "veec/source/Identifier.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/decl/DeclarationNode.hpp"
#include "veec/symbols/SymbolFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief This node represents a single parameter declaration in the AST.
 */
class ParameterDeclNode : public DeclarationNode {
public:
	symbols::VariableSymbol* symbol = nullptr;

    /**
     * @brief Creates a new ParameterDeclNode with a given name and type.
     * @param name The name of this parameter declaration.
     * @param type The type of this parameter declaration.
     */
    ParameterDeclNode(
        AstKey,
        source::Identifier name,
        TypeNode* type
    )
        : DeclarationNode(AstKey{}, AstKind::ParameterDecl),
        _name(name),
        _type(type) {}

    virtual ~ParameterDeclNode() = default;

    /**
     * @brief Gets the name of this parameter declaration.
     * @return The name of this parameter declaration.
     */
    inline const source::Identifier& getName() const {
        return _name;
    }
    /**
     * @brief Gets the type of this parameter declaration (read-only).
     * @return The type of this parameter declaration.
     */
    inline const TypeNode* getType() const {
        return _type;
    }
    /**
     * @brief Gets the type of this parameter declaration.
     * @return The type of this parameter declaration.
     */
    inline TypeNode* getType() {
        return _type;
    }

    /**
     * @brief Checks if the given AST node is a ParameterDeclNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a ParameterDeclNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::ParameterDecl;
    }

private:
    source::Identifier _name;
    TypeNode* _type;
};

} // namespace ast
VEEC_NAMESPACE_END
