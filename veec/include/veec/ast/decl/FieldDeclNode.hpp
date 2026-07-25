/**
 * @file FieldDeclNode.hpp
 * @brief This file contains the definition of the FieldDeclNode AST node.
 * The FieldDecl node represents a single field declaration in the AST.
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
 * @brief This node represents a single field declaration in the AST.
 */
class FieldDeclNode : public DeclarationNode {
public:
	symbols::FieldSymbol* symbol = nullptr;

    /**
     * @brief Creates a new FieldDeclNode with a given name and type.
     * @param name The name of this field declaration.
     * @param type The type of this field declaration.
     */
    FieldDeclNode(
        AstKey,
        source::Identifier name,
        TypeNode* type
    )
        : DeclarationNode(AstKey{}, AstKind::FieldDecl),
        _name(name),
        _type(type) {}

    virtual ~FieldDeclNode() = default;

    /**
     * @brief Gets the name of this field declaration.
     * @return The name of this field declaration.
     */
    inline const source::Identifier& getName() const {
        return _name;
    }
    
    /**
     * @brief Gets the type of this field declaration (read-only).
     * @return The type of this field declaration.
     */
    inline const TypeNode* getType() const {
        return _type;
    }
    /**
     * @brief Gets the type of this field declaration.
     * @return The type of this field declaration.
     */
    inline TypeNode* getType() {
        return _type;
    }

    /**
     * @brief Checks if the given AST node is a FieldDeclNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a FieldDeclNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::FieldDecl;
    }

private:
    source::Identifier _name;
    TypeNode* _type;
};

} // namespace ast
VEEC_NAMESPACE_END
