/**
 * @file IntLiteralExprNode.hpp
 * @brief This file contains the definition of the IntLiteralExprNode AST node.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/BigInt.hpp"
#include "veec/basic/Token.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/expr/LiteralExprNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @class IntLiteralExprNode
 * @brief Represents an integer literal expression in the AST.
 */
class IntLiteralExprNode : public LiteralExprNode {
public:
	IntLiteralExprNode(AstKey, Token valueToken, basic::BigInt value)
		: LiteralExprNode(AstKey{}, AstKind::IntLiteralExpr, std::move(valueToken)),
		  _value(std::move(value)) {}

	virtual ~IntLiteralExprNode() = default;

	inline const basic::BigInt& getValue() const {
		return _value;
	}

	inline LiteralType getLiteralType() const override {
		return LiteralType::Integer;
	}
	inline const basic::BigInt& getIntegerValue() const override {
		return _value;
	}

	static bool isClassOf(const AstNode* node) {
		return node && node->getNodeKind() == AstKind::IntLiteralExpr;
	}

private:
	basic::BigInt _value;
};

} // namespace ast
VEEC_NAMESPACE_END
