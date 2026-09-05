/**
 * @file CBinaryExpr.hpp
 * @brief This file contains the definition of the CBinaryExpr struct which represents
 * a C binary expression construct.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/CConstruct.hpp"
#include "veec/codegen/c/construct/expr/CExpr.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @enum CBinaryOperator
 * @brief Enumerates all possible C binary operators.
 */
enum class CBinaryOperator {
    Add, // +
    Sub, // -
    Mul, // *
    Div, // /
    Mod, // %

    BitAnd, // &
    BitOr, // |
    BitXor, // ^
    Shl, // <<
    Shr, // >>

    LogicalAnd, // &&
    LogicalOr, // ||

    Eq, // ==
    Ne, // !=
    Lt, // <
    Le, // <=
    Gt, // >
    Ge, // >=
};

class CBinaryExpr : public CExpr {
public:
    CBinaryOperator op;
    CExpr* lhs = nullptr;
    CExpr* rhs = nullptr;

    virtual ~CBinaryExpr() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
