/**
 * @file CUnaryExpr.hpp
 * @brief This file contains the definition of the CUnaryExpr struct which represents
 * a C unary expression construct.
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
 * @enum CUnaryOperator
 * @brief Enumerates all possible C (prefix) unary operators.
 */
enum class CUnaryOperator {
    Neg, // -x
    BitNot, // ~x
    LogicalNot, // !x
    Deref, // *x
    AddrOf, // &x
};

class CUnaryExpr : public CExpr {
public:
    CUnaryOperator op;
    CExpr* operand = nullptr;

    virtual ~CUnaryExpr() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
