/**
 * @file ModuleDeclNode.hpp
 * @brief This file contains the definition of the ModuleDeclNode AST node.
 * The ModuleDecl node represents a single module declaration in the AST.
 */

#pragma once

#include <vector>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/source/Identifier.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/decl/DeclarationNode.hpp"
#include "veec/symbols/SymbolFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief This node represents a single module declaration in the AST.
 */
class ModuleDeclNode : public DeclarationNode {
public:
	symbols::ModuleSymbol* symbol = nullptr;

    ModuleDeclNode(
        AstKey,
        source::Identifier name,
        std::vector<ItemNode*>&& items
    )
        : DeclarationNode(AstKey{}, AstKind::ModuleDecl),
        _name(name),
        _items(std::move(items)) {}

    virtual ~ModuleDeclNode() = default;

    /**
     * @brief Gets the name of this module declaration.
     * @return The name of this module declaration.
     */
    inline const source::Identifier& getName() const {
        return _name;
    }

    /**
     * @brief Gets the member items of this module declaration (read-only).
     * @return The member items of this module declaration.
     */
    inline const std::vector<ItemNode*>& getItems() const {
        return _items;
    }
    /**
     * @brief Gets the member items of this module declaration.
     * @return The member items of this module declaration.
     */
    inline std::vector<ItemNode*>& getItems() {
        return _items;
    }

    /**
     * @brief Checks if the given AST node is a ModuleDeclNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a ModuleDeclNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::ModuleDecl;
    }

private:
    source::Identifier _name;
    std::vector<ItemNode*> _items;
};

} // namespace ast
VEEC_NAMESPACE_END
