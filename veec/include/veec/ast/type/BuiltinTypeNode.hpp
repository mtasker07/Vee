/**
 * @file BuiltinTypeNode.hpp
 * @brief This file contains the definition of the BuiltinTypeNode AST node.
 * The BuiltinType node represents a built-in type in the AST.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/type/TypeNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief Represents the kind of a built-in type in the AST.
 */
enum class BuiltinTypeKind : u8 {
    Void,
    Bool,
    String,
    I8,
    U8,
    I16,
    U16,
    I32,
    U32,
    I64,
    U64,
    F32,
    F64,
};

/**
 * @brief This node represents a built-in type in the AST.
 */
class BuiltinTypeNode : public TypeNode {
public:
    BuiltinTypeNode(AstKey, BuiltinTypeKind kind)
        : TypeNode(AstKey{}, AstKind::BuiltinType), _kind(kind) {}

    virtual ~BuiltinTypeNode() = default;

    /**
     * @brief Gets the kind of this built-in type.
     * @return The kind of this built-in type.
     */
    inline BuiltinTypeKind getBuiltinTypeKind() const {
        return _kind;
    }

    /**
     * @brief Checks if the given AST node is a BuiltinTypeNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a BuiltinTypeNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::BuiltinType;
    }

private:
    BuiltinTypeKind _kind;
};

} // namespace ast
VEEC_NAMESPACE_END
