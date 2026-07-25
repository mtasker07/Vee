/**
 * @file QualifiedNameNode.hpp
 * @brief This file contains the definition of the QualifiedNameNode AST node.
 * The QualifiedNameNode represents a qualified name in the program, for example,
 * `Foo::Bar::Baz<i32>`.
 * 
 * It is made up of segments, each segment containing a required identifier and an optional
 * list of template arguments so that each segment can represent a template instantiation,
 * for example, `Foo<i32>::Bar::Baz<f32>`.
 * 
 * This node is the basic name node, however, when used in expression contexts, it is wrapped
 * in a NameExprNode, for example in `Foo::Bar()`, which is a call expression with a NameExprNode
 * as the callee.
 * 
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/source/Identifier.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/symbols/SymbolFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief Represents a single segment of a qualified name,
 * which consists of an identifier and an optional list of template arguments.
 */
struct QualifiedNameSegment {
    /**
     * @brief The identifier for this segment of the qualified name.
     */
    source::Identifier identifier;
    /**
     * @brief The optional generic arguments for this segment of the qualified name.
     */
    GenericArgsNode* genericArgs = nullptr;
};

/**
 * @class QualifiedNameNode
 * @brief Qualified names represent a sequence of identifiers forming a fully qualified name.
 */
class QualifiedNameNode : public AstNode {
public:
    /**
     * @brief The symbol that this qualified name resolves to, if any.
     * This is set during the symbol resolution pass.
     */
    symbols::Symbol* resolvedSymbol = nullptr;

    /**
     * @brief Constructs a new qualified name node with the given segments.
     * @param segments The segments forming this qualified name.
     */
    QualifiedNameNode(AstKey, std::vector<QualifiedNameSegment> segments)
        : AstNode(AstKey{}, AstKind::QualifiedName), _segments(std::move(segments)) {
        VEE_ASSERT(!_segments.empty(), "Qualified name must have at least one segment");
    }

    virtual ~QualifiedNameNode() = default;

    /**
     * @brief Gets the segments forming this qualified name (read-only).
     * @return The segments forming this qualified name.
     */
    inline const std::vector<QualifiedNameSegment>& getSegments() const { return _segments; }

    /**
     * @brief Checks if the given AST node is a QualifiedNameNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a QualifiedNameNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::QualifiedName;
    }

private:
    std::vector<QualifiedNameSegment> _segments;
};

} // namespace ast
VEEC_NAMESPACE_END
