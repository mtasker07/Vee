/**
 * @file MethodDeclNode.hpp
 * @brief This file contains the definition of the MethodDeclNode AST node.
 * The MethodDecl node represents a single method declaration in the AST.
 * 
 * Note that a method declaration is essentially identical to a function declaration,
 * but is any function within a user-defined type. A method also has more object-oriented
 * semantics, such as an optional self parameter.
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
 * @brief This node represents a single method declaration in the AST.
 */
class MethodDeclNode : public DeclarationNode {
public:
	symbols::FunctionSymbol* symbol = nullptr;

    MethodDeclNode(
        AstKey,
        source::Identifier name,
        std::vector<ParameterDeclNode*> parameters,
        TypeNode* returnType,
        BlockStmtNode* body
    )
        : DeclarationNode(AstKey{}, AstKind::MethodDecl),
        _name(name),
        _parameters(std::move(parameters)),
        _returnType(returnType),
        _body(body) {}

    virtual ~MethodDeclNode() = default;

    /**
     * @brief Gets the name of this method declaration.
     * @return The name of this method declaration.
     */
    inline const source::Identifier& getName() const {
        return _name;
    }
    /**
     * @brief Gets the parameters of this method declaration (read-only).
     * @return The parameters of this method declaration.
     */
    inline const std::vector<ParameterDeclNode*>& getParameters() const {
        return _parameters;
    }
    /**
     * @brief Gets the parameters of this method declaration.
     * @return The parameters of this method declaration.
     */
    inline std::vector<ParameterDeclNode*>& getParameters() {
        return _parameters;
    }
    /**
     * @brief Gets the return type of this method declaration (read-only).
     * @return The return type of this method declaration.
     */
    inline const TypeNode* getReturnType() const {
        return _returnType;
    }
    /**
     * @brief Gets the return type of this method declaration.
     * @return The return type of this method declaration.
     */
    inline TypeNode* getReturnType() {
        return _returnType;
    }
    /**
     * @brief Gets the body of this method declaration (read-only).
     * @return The body of this method declaration.
     */
    inline const BlockStmtNode* getBody() const {
        return _body;
    }
    /**
     * @brief Gets the body of this method declaration.
     * @return The body of this method declaration.
     */
    inline BlockStmtNode* getBody() {
        return _body;
    }

    /**
     * @brief Checks if the given AST node is a MethodDeclNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a MethodDeclNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::MethodDecl;
    }

private:
    source::Identifier _name;
    std::vector<ParameterDeclNode*> _parameters;
    TypeNode* _returnType;
    BlockStmtNode* _body;
};

} // namespace ast
VEEC_NAMESPACE_END
