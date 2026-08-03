/**
 * @file AstContext.hpp
 * @brief This file contains the definition of the ASTContext struct,
 * which is used to hold context for AST-related operations during the
 * compilation process.
 */

#pragma once

#include <vector>
#include <type_traits>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Arena.hpp"
#include "veec/source/SourceRange.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/CompilationUnitNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @class AstContext
 * @brief Used to hold context for AST-related operations during the compilation process.
 */
class AstContext {
public:
    std::vector<CompilationUnitNode*> modules = {};

    /**
     * @brief Creates a new ASTContext instance.
     */
    AstContext() = default;
    ~AstContext() = default;

    /**
     * @brief Creates a new AST node of type T with the given range.
     * @tparam T The type of AST node to create.
     * @param range The source range that corresponds to the AST node being created.
     * @param args The arguments to forward to the constructor.
     * @return A pointer to the created AST node.
     */
    template<typename T, typename... Args>
    inline T* makeNode(const source::SourceRange& range, Args&&... args) {
        static_assert(std::is_base_of_v<AstNode, T>, "T must be derived from AstNode");

        auto node = _nodeArena.create<T>(typename AstNode::AstKey{}, std::forward<Args>(args)...);
        node->_range = range;
        return node;
    }

private:
    basic::Arena<> _nodeArena;
};

} // namespace ast
VEEC_NAMESPACE_END
