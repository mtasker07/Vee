/**
 * @file StringLiteralExprNode.hpp
 * @brief This file contains the definition of the StringLiteralExprNode AST node.
 */

#pragma once

#include <string>
#include <string_view>

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
 * @class StringLiteralExprNode
 * @brief Represents a string literal expression in the AST.
 */
class StringLiteralExprNode : public LiteralExprNode {
public:
	StringLiteralExprNode(AstKey, Token valueToken, std::string value)
		: LiteralExprNode(AstKey{}, AstKind::StringLiteralExpr, std::move(valueToken)),
		  _value(std::move(value)) {}

	virtual ~StringLiteralExprNode() = default;

	inline const std::string& getValue() const {
		return _value;
	}
	inline std::string& getValue() {
		return _value;
	}

	inline LiteralType getLiteralType() const override {
		return LiteralType::String;
	}
	inline std::string_view getStringValue() const override {
		return _value;
	}

	static bool isClassOf(const AstNode* node) {
		return node && node->getNodeKind() == AstKind::StringLiteralExpr;
	}

private:
	std::string _value;
};

} // namespace ast
VEEC_NAMESPACE_END
