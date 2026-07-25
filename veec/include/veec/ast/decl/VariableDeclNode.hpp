/**
 * @file VariableDeclNode.hpp
 * @brief This file contains the definition of the VariableDeclNode AST node.
 * The VariableDecl node represents a single variable declaration in the AST.
 */

#pragma once

#include <vector>
#include <string_view>

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
 * @brief The kind of a variable declaration, indicating whether it was declared with `let` or `var`.
 */
enum class VariableDeclKind : u8 {
    Let,
    Var,
};

/**
 * @brief This node represents a single variable declaration in the AST.
 */
class VariableDeclNode : public DeclarationNode {
public:
	symbols::VariableSymbol* symbol = nullptr;

    VariableDeclNode(
        AstKey,
        VariableDeclKind kind,
        source::Identifier name,
        TypeNode* type,
        ExpressionNode* initializer
    )
        : DeclarationNode(AstKey{}, AstKind::VariableDecl),
		_declKind(kind),
        _name(name),
        _type(type),
        _initializer(initializer) {}

    virtual ~VariableDeclNode() = default;

    /**
     * @brief Gets the kind of this variable declaration (whether it was declared with `let` or `var`).
     * @return The kind of this variable declaration.
     */
    inline VariableDeclKind getDeclKind() const {
        return _declKind;
    }

    /**
     * @brief Gets the name of this variable declaration.
     * @return The name of this variable declaration.
     */
    inline const source::Identifier& getName() const {
        return _name;
    }
    /**
     * @brief Gets the type of this variable declaration (read-only).
     * @return The type of this variable declaration.
     */
    inline const TypeNode* getType() const {
        return _type;
    }
    /**
     * @brief Gets the type of this variable declaration.
     * @return The type of this variable declaration.
     */
    inline TypeNode* getType() {
        return _type;
    }
    /**
     * @brief Gets the initializer of this variable declaration (read-only).
     * @return The initializer of this variable declaration.
     */
    inline const ExpressionNode* getInitializer() const {
        return _initializer;
    }
    /**
     * @brief Gets the initializer of this variable declaration.
     * @return The initializer of this variable declaration.
     */
    inline ExpressionNode* getInitializer() {
        return _initializer;
    }

    /**
     * @brief Checks if the given AST node is a VariableDeclNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a VariableDeclNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::VariableDecl;
    }

private:
	VariableDeclKind _declKind;
    source::Identifier _name;
	TypeNode* _type;
	ExpressionNode* _initializer;
};

} // namespace ast
VEEC_NAMESPACE_END
