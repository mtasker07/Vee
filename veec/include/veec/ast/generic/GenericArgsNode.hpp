/**
 * @file GenericArgsNode.hpp
 * @brief This file contains the definition of the GenericArgsNode AST node.
 * The GenericArgsNode represents a list of generic arguments in the program, for example,
 * `<i32, f32>`.
 * 
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/source/Identifier.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @class GenericArgsNode
 * @brief Represents a list of generic arguments in the AST.
 */
class GenericArgsNode : public AstNode {
public:
    /**
     * @brief Constructs a new generic arguments node with the given arguments.
     * @param args The generic arguments.
     */
    GenericArgsNode(AstKey, std::vector<TypeNode*> args)
        : AstNode(AstKey{}, AstKind::GenericArgs), _args(std::move(args)) {}

    virtual ~GenericArgsNode() = default;

    /**
     * @brief Gets the generic arguments (read-only).
     * @return The generic arguments.
     */
    inline const std::vector<TypeNode*>& getArgs() const { return _args; }
    /**
     * @brief Gets the generic arguments.
     * @return The generic arguments.
     */
    inline std::vector<TypeNode*>& getArgs() { return _args; }

    /**
     * @brief Checks if the given AST node is a GenericArgsNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a GenericArgsNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::GenericArgs;
    }

private:
    std::vector<TypeNode*> _args;
};

} // namespace ast
VEEC_NAMESPACE_END
