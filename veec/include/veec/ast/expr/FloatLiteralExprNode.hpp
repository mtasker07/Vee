/**
 * @file FloatLiteralExprNode.hpp
 * @brief This file contains the definition of the FloatLiteralExprNode AST node.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Token.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/expr/LiteralExprNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @class FloatLiteralExprNode
 * @brief Represents a floating-point literal expression in the AST.
 */
class FloatLiteralExprNode : public LiteralExprNode {
public:
	FloatLiteralExprNode(AstKey, Token valueToken, double value)
		: LiteralExprNode(AstKey{}, AstKind::FloatLiteralExpr, std::move(valueToken)),
		  _value(value) {}

	virtual ~FloatLiteralExprNode() = default;

	inline double getValue() const {
		return _value;
	}

	inline LiteralType getLiteralType() const override {
		return LiteralType::Float;
	}
	inline double getFloatValue() const override {
		return _value;
	}

	static bool isClassOf(const AstNode* node) {
		return node && node->getNodeKind() == AstKind::FloatLiteralExpr;
	}

private:
	double _value;
};

} // namespace ast
VEEC_NAMESPACE_END
