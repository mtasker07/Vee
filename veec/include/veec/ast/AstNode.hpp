/**
 * @file AstNode.hpp
 * @brief This file contains the definition of the base AST node
 * class used in the Vee compiler.
 * 
 * All AST nodes inherit from this class, which provides common functionality
 * such as type identification and source tracking.
 * 
 * An AstKey system is used to restrict construction of AST nodes to the
 * AstContext::makeNode helper function ONLY. Which ensures that the AST nodes are always
 * setup correctly with necessary information such as the source range.
 * ALL children of AstNode must have a constructor that takes an AstKey as the
 * first argument!!
 */

#pragma once

#include <string>
#include <memory>
#include <utility>
#include <type_traits>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/source/SourceRange.hpp"
#include "veec/ast/AstKind.hpp"

VEEC_NAMESPACE_BEGIN

namespace compilation {
    class CompilationContext;
}

namespace ast {

/**
 * @brief Base class for all AST nodes.
 */
class AstNode {
public:
    // AST nodes cannot be copied.
    // TODO: Consider making a clone() method.
    AstNode(const AstNode&) = delete;
    AstNode& operator=(const AstNode&) = delete;

    // Allow move
    AstNode(AstNode&&) = default;
    AstNode& operator=(AstNode&&) = default;

    virtual ~AstNode() = default;

    /**
     * @brief Gets the kind of this AST node.
     * @return The kind of this AST node.
     */
    inline AstKind getNodeKind() const {
        return _kind;
    }
    /**
     * @brief Gets the source range that corresponds to this AST node.
     * @return The source range that corresponds to this AST node.
     */
    inline const source::SourceRange& getRange() const {
        return _range;
    }

    /**
     * @brief Converts this AST node to a human-readable string representation for debugging and informational purposes.
     * @return A human-readable string representation of this AST node.
     * @note This method internally wraps AstPrinter and is mostly for convenience,
     * if you want more control it is recommended to use AstPrinter directly.
     */
    std::string toString(const compilation::CompilationContext& ctx) const;

protected:
    struct AstKey {};
    
    AstNode(AstKey, AstKind kind) : _kind(kind) {};

private:
    // For construction
    friend class AstContext;

    AstKind _kind;
    source::SourceRange _range;
};

/**
 * @brief Helper function to safely cast an AST node to a specific type.
 * Always use this over dynamic_cast as its significantly faster!!
 * @tparam T The type to cast to, which must be derived from AstNode.
 * @param node The AST node to cast.
 * @return A pointer to the casted AST node if the cast is valid, or nullptr if the cast is invalid.
 */
template<typename T>
inline T* ast_cast(AstNode* node) {
    if (node && T::isClassOf(node))
        return static_cast<T*>(node);
    return nullptr;
}

} // namespace ast
VEEC_NAMESPACE_END
