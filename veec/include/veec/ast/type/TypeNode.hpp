/**
 * @file TypeNode.hpp
 * @brief This file contains the definition of the TypeNode AST node.
 * The Type node represents a type in the AST.
 */

#pragma once

#include <vector>
#include <memory>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief This node represents a syntax type in the AST.
 */
class TypeNode : public AstNode {
public:
    virtual ~TypeNode() = default;

    /**
     * @brief Checks if the given AST node is a TypeNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a TypeNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        if (!node) return false;

        AstKind kind = node->getNodeKind();
        return
            kind >= AstKind::FirstType &&
            kind <= AstKind::LastType;
    }

protected:
    /**
     * @brief Creates an new type node with a given kind.
     * @param kind The kind of this type node.
     */
    TypeNode(AstKey, AstKind kind)
        : AstNode(AstKey{}, kind) {}
};

} // namespace ast
VEEC_NAMESPACE_END
