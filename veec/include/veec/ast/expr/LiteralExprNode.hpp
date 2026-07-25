/**
 * @file LiteralExprNode.hpp
 * @brief This file contains the definition of the LiteralExprNode AST node.
 * The LiteralExprNode represents a single literal expression in the program.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Token.hpp"
#include "veec/basic/BigInt.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/expr/ExpressionNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @enum LiteralType
 * @brief Represents the semantic type of a literal expression.
 */
enum class LiteralType : u8 {
    Integer,
    Float,
    String,
    Bool,
};

/**
 * @class LiteralExprNode
 * @brief Represents a literal expression in the AST. For example an integer literal.
 */
class LiteralExprNode : public ExpressionNode {
public:
    virtual ~LiteralExprNode() = default;

    /**
     * @brief Gets token of this literal expression.
     * @return Token of this literal expression.
     */
    inline const Token& getLiteralToken() const {
        return _valueToken;
    }
    /**
     * @brief Gets token of this literal expression.
     * @return Token of this literal expression.
     */
    inline Token& getLiteralToken() {
        return _valueToken;
    }

    /**
     * @brief Gets semantic type of this literal expression.
     * @return Literal semantic type.
     */
    virtual LiteralType getLiteralType() const {
        VEE_UNREACHABLE("getLiteralType() called for base literal expression");
    }

    /**
     * @brief Gets integer value for integer literals.
     * @return Integer value.
     */
    virtual const basic::BigInt& getIntegerValue() const {
        VEE_FATAL("getIntegerValue() called for non-integer literal");
    }
    /**
     * @brief Gets float value for float literals.
     * @return Float value.
     */
    virtual double getFloatValue() const {
        VEE_FATAL("getFloatValue() called for non-float literal");
    }
    /**
     * @brief Gets string value for string literals.
     * @return String value.
     */
    virtual std::string_view getStringValue() const {
        VEE_FATAL("getStringValue() called for non-string literal");
    }
    /**
     * @brief Gets boolean value for boolean literals.
     * @return Boolean value.
     */
    virtual bool getBoolValue() const {
        VEE_FATAL("getBoolValue() called for non-boolean literal");
    }

    /**
     * @brief Checks if the given AST node is a LiteralExprNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a LiteralExprNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && (
            node->getNodeKind() == AstKind::IntLiteralExpr ||
            node->getNodeKind() == AstKind::FloatLiteralExpr ||
            node->getNodeKind() == AstKind::StringLiteralExpr ||
            node->getNodeKind() == AstKind::BoolLiteralExpr
        );
    }

protected:
    LiteralExprNode(AstKey, AstKind kind, Token valueToken)
        : ExpressionNode(AstKey{}, kind),
          _valueToken(std::move(valueToken)) {}

private:
    Token _valueToken;
};

} // namespace ast
VEEC_NAMESPACE_END
