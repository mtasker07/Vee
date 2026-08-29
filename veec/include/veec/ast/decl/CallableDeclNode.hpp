/**
 * @file CallableDeclNode.hpp
 * @brief This file contains the definition of the CallableDeclNode AST node.
 * The CallableDecl node represents a single callable declaration in the AST.
 */

#pragma once

#include <vector>
#include <string_view>
#include <optional>

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
 * @brief Base class for all invocable (callable) declaration nodes in the AST.
 */
class CallableDeclNode : public DeclarationNode {
public:
    symbols::FunctionSymbol* symbol = nullptr;

    virtual ~CallableDeclNode() = default;

    /**
     * @brief Gets the name of this callable declaration.
     * @return The name of this callable declaration.
     */
    inline const source::Identifier& getName() const {
        return _name;
    }
    /**
     * @brief Gets the generic type parameters of this callable declaration (read-only).
     * @return The generic type parameters of this callable declaration.
     */
    inline const GenericArgsNode* getGenericParams() const {
        return _genericParameters;
    }
    /**
     * @brief Gets the generic type parameters of this callable declaration.
     * @return The generic type parameters of this callable declaration.
     */
    inline GenericArgsNode* getGenericParams() {
        return _genericParameters;
    }
    /**
     * @brief Gets the parameters of this callable declaration (read-only).
     * @return The parameters of this callable declaration.
     */
    inline const std::vector<ParameterDeclNode*>& getParameters() const {
        return _parameters;
    }
    /**
     * @brief Gets the parameters of this callable declaration.
     * @return The parameters of this callable declaration.
     */
    inline std::vector<ParameterDeclNode*>& getParameters() {
        return _parameters;
    }
    /**
     * @brief Gets the return type of this callable declaration (read-only).
     * @return The return type of this callable declaration.
     */
    inline const TypeNode* getReturnType() const {
        return _returnType;
    }
    /**
     * @brief Gets the return type of this callable declaration.
     * @return The return type of this callable declaration.
     */
    inline TypeNode* getReturnType() {
        return _returnType;
    }
    /**
     * @brief Gets the body of this callable declaration (read-only).
     * @return The body of this callable declaration.
     */
    inline const BlockStmtNode* getBody() const {
        return _body;
    }
    /**
     * @brief Gets the body of this callable declaration.
     * @return The body of this callable declaration.
     */
    inline BlockStmtNode* getBody() {
        return _body;
    }

    /**
     * @brief Checks if the given AST node is a CallableDeclNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a CallableDeclNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        if (!node) return false;

        AstKind kind = node->getNodeKind();
        return
            kind == AstKind::FunctionDecl ||
            kind == AstKind::MethodDecl;
    }

protected:
    /**
     * @brief Creates a new CallableDeclNode instance.
     * @param kind The kind of this callable declaration node.
     * @param name The name of this callable declaration.
     * @param genericParameters The generic type parameters of this callable declaration.
     * @param parameters The parameters of this callable declaration.
     * @param returnType The return type of this callable declaration.
     * @param body The body of this callable declaration.
     */
    CallableDeclNode(
        AstKey,
        AstKind kind,
        source::Identifier name,
        GenericArgsNode* genericParameters,
        std::vector<ParameterDeclNode*> parameters,
        TypeNode* returnType,
        BlockStmtNode* body
    )
        : DeclarationNode(AstKey{}, kind),
        _name(name),
        _genericParameters(genericParameters),
        _parameters(std::move(parameters)),
        _returnType(returnType),
        _body(body) {}

private:
    source::Identifier _name;
    GenericArgsNode* _genericParameters;
    std::vector<ParameterDeclNode*> _parameters;
    TypeNode* _returnType;
    BlockStmtNode* _body;
};

} // namespace ast
VEEC_NAMESPACE_END
