/**
 * @file FunctionDeclNode.hpp
 * @brief This file contains the definition of the FunctionDeclNode AST node.
 * The FunctionDecl node represents a single function declaration in the AST.
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
 * @brief This node represents a single function declaration in the AST.
 */
class FunctionDeclNode : public DeclarationNode {
public:
	symbols::FunctionSymbol* symbol = nullptr;

    FunctionDeclNode(
        AstKey,
        source::Identifier name,
        GenericArgsNode* genericParameters,
        std::vector<ParameterDeclNode*> parameters,
        TypeNode* returnType,
        BlockStmtNode* body
    )
        : DeclarationNode(AstKey{}, AstKind::FunctionDecl),
        _name(name),
        _parameters(std::move(parameters)),
        _returnType(returnType),
        _body(body) {}

    virtual ~FunctionDeclNode() = default;

    /**
     * @brief Gets the name of this function declaration.
     * @return The name of this function declaration.
     */
    inline const source::Identifier& getName() const {
        return _name;
    }
    /**
     * @brief Gets the generic type parameters of this function declaration (read-only).
     * @return The generic type parameters of this function declaration.
     */
    inline const GenericArgsNode* getGenericParams() const {
        return _genericParameters;
    }
    /**
     * @brief Gets the generic type parameters of this function declaration.
     * @return The generic type parameters of this function declaration.
     */
    inline GenericArgsNode* getGenericParams() {
        return _genericParameters;
    }
    /**
     * @brief Gets the parameters of this function declaration (read-only).
     * @return The parameters of this function declaration.
     */
    inline const std::vector<ParameterDeclNode*>& getParameters() const {
        return _parameters;
    }
    /**
     * @brief Gets the parameters of this function declaration.
     * @return The parameters of this function declaration.
     */
    inline std::vector<ParameterDeclNode*>& getParameters() {
        return _parameters;
    }
    /**
     * @brief Gets the return type of this function declaration (read-only).
     * @return The return type of this function declaration.
     */
    inline const TypeNode* getReturnType() const {
        return _returnType;
    }
    /**
     * @brief Gets the return type of this function declaration.
     * @return The return type of this function declaration.
     */
    inline TypeNode* getReturnType() {
        return _returnType;
    }
    /**
     * @brief Gets the body of this function declaration (read-only).
     * @return The body of this function declaration.
     */
    inline const BlockStmtNode* getBody() const {
        return _body;
    }
    /**
     * @brief Gets the body of this function declaration.
     * @return The body of this function declaration.
     */
    inline BlockStmtNode* getBody() {
        return _body;
    }

    /**
     * @brief Checks if the given AST node is a FunctionDeclNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a FunctionDeclNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::FunctionDecl;
    }

private:
    source::Identifier _name;
    GenericArgsNode* _genericParameters;
    std::vector<ParameterDeclNode*> _parameters;
    TypeNode* _returnType;
    BlockStmtNode* _body;
};

} // namespace ast
VEEC_NAMESPACE_END
