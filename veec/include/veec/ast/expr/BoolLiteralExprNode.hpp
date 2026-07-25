/**
 * @file BoolLiteralExprNode.hpp
 * @brief This file contains the definition of the BoolLiteralExprNode AST node.
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
 * @class BoolLiteralExprNode
 * @brief Represents a boolean literal expression in the AST.
 */
class BoolLiteralExprNode : public LiteralExprNode {
public:
	BoolLiteralExprNode(AstKey, Token valueToken, bool value)
		: LiteralExprNode(AstKey{}, AstKind::BoolLiteralExpr, std::move(valueToken)),
		  _value(value) {}

	virtual ~BoolLiteralExprNode() = default;

	inline bool getValue() const {
		return _value;
	}

	inline LiteralType getLiteralType() const override {
		return LiteralType::Bool;
	}
	inline bool getBoolValue() const override {
		return _value;
	}

	static bool isClassOf(const AstNode* node) {
		return node && node->getNodeKind() == AstKind::BoolLiteralExpr;
	}

private:
	bool _value;
};

} // namespace ast
VEEC_NAMESPACE_END
