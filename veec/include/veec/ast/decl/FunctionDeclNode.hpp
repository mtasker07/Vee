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
#include "veec/ast/decl/CallableDeclNode.hpp"
#include "veec/symbols/SymbolFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief This node represents a single function declaration in the AST.
 */
class FunctionDeclNode : public CallableDeclNode {
public:
    FunctionDeclNode(
        AstKey,
        source::Identifier name,
        GenericArgsNode* genericParameters,
        std::vector<ParameterDeclNode*> parameters,
        TypeNode* returnType,
        BlockStmtNode* body
    )
        : CallableDeclNode(
            AstKey{},
            AstKind::FunctionDecl,
            name,
            genericParameters,
            std::move(parameters),
            returnType,
            body
        ) {}

    virtual ~FunctionDeclNode() = default;

    /**
     * @brief Checks if the given AST node is a FunctionDeclNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a FunctionDeclNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::FunctionDecl;
    }
};

} // namespace ast
VEEC_NAMESPACE_END
