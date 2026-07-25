/**
 * @file CompilationUnitNode.hpp
 * @brief This file contains the definition of the CompilationUnitNode AST node.
 * The CompilationUnit node represents a single compilation file and contains a list
 * of all top-level items.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/ItemNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief This node represents a single compilation file and contains a list of all top-level items.
 */
class CompilationUnitNode : public AstNode {
public:
    /**
     * @brief Creates an empty compilation unit node.
     */
    CompilationUnitNode(AstKey)
        : AstNode(AstKey{}, AstKind::CompilationUnit) {}
    /**
     * @brief Creates a compilation unit node with the given list of items.
     * @param items The list of items that make up this compilation unit.
     */
    CompilationUnitNode(AstKey, std::vector<ItemNode*> items)
        : AstNode(AstKey{}, AstKind::CompilationUnit), _items(std::move(items)) {}

    virtual ~CompilationUnitNode() = default;

    /**
     * @brief Adds an item to the compilation unit.
     * @param node The item to add to the compilation unit.
     */
    void addItem(ItemNode* node) {
        _items.push_back(node);
    }
    
    /**
     * @brief Gets the list of items in this compilation unit (read-only).
     * @return The list of items in this compilation unit.
     */
    const std::vector<ItemNode*>& getItems() const {
        return _items;
    }
    /**
     * @brief Gets the list of items in this compilation unit.
     * @return The list of items in this compilation unit.
     */
    std::vector<ItemNode*>& getItems() {
        return _items;
    }

    /**
     * @brief Checks if the given AST node is a CompilationUnitNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a CompilationUnitNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::CompilationUnit;
    }

private:
    std::vector<ItemNode*> _items;
};

} // namespace ast
VEEC_NAMESPACE_END
