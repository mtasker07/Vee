/**
 * @file MirNode.hpp
 * @brief This file contains the definition of the base MIR node
 * class used in the Vee compiler.
 * 
 * All MIR nodes inherit from this class, which provides common functionality shared among
 * all MIR nodes.
 * 
 * Unlike AST nodes, we don't store any source information.
 */

#pragma once

#include <string>
#include <memory>
#include <utility>
#include <type_traits>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/mir/MirKind.hpp"

VEEC_NAMESPACE_BEGIN

namespace compilation {
    class CompilationContext;
}

namespace mir {

/**
 * @brief Base class for all MIR nodes.
 */
class MirNode {
public:
    // MIR nodes cannot be copied.
    MirNode(const MirNode&) = delete;
    MirNode& operator=(const MirNode&) = delete;

    // Allow move
    MirNode(MirNode&&) = default;
    MirNode& operator=(MirNode&&) = default;

    virtual ~MirNode() = default;

    /**
     * @brief Gets the kind of this MIR node.
     * @return The kind of this MIR node.
     */
    inline MirKind getNodeKind() const {
        return _kind;
    }

    /**
     * @brief Converts this MIR node to a human-readable string representation for debugging and
     * informational purposes.
     * @return A human-readable string representation of this MIR node.
     * @note This method internally wraps MirPrinter and is mostly for convenience,
     * if you want more control it is recommended to use MirPrinter directly.
     */
    std::string toString(const compilation::CompilationContext& ctx) const;

protected:
    struct MirKey {};
    
    MirNode(MirKey, MirKind kind) : _kind(kind) {}

private:
    // For construction
    friend class MirFactory;

    MirKind _kind;
};

/**
 * @brief Helper function to safely cast a MIR node to a specific type.
 * Always use this over dynamic_cast as its significantly faster!!
 * @tparam T The type to cast to, which must be derived from MirNode.
 * @param node The MIR node to cast.
 * @return A pointer to the casted MIR node if the cast is valid, or nullptr if the cast is invalid.
 */
template<typename T>
inline T* mir_cast(MirNode* node) {
    if (node && T::isClassOf(node))
        return static_cast<T*>(node);
    return nullptr;
}

} // namespace mir
VEEC_NAMESPACE_END
