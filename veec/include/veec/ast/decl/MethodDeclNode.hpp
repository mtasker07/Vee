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
#include "veec/ast/decl/CallableDeclNode.hpp"
#include "veec/symbols/SymbolFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief This node represents a single method declaration in the AST.
 */
class MethodDeclNode : public CallableDeclNode {
public:
    MethodDeclNode(
        AstKey,
        source::Identifier name,
        std::vector<ParameterDeclNode*> parameters,
        TypeNode* returnType,
        BlockStmtNode* body
    )
        : CallableDeclNode(
            AstKey{},
            AstKind::MethodDecl,
            name,
            nullptr, // TODO: Generic parameters in methods
            std::move(parameters),
            returnType,
            body
        ) {}

    virtual ~MethodDeclNode() = default;

    /**
     * @brief Checks if the given AST node is a MethodDeclNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a MethodDeclNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::MethodDecl;
    }
};

} // namespace ast
VEEC_NAMESPACE_END
