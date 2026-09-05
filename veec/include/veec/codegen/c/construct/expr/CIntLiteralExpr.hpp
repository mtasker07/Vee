/**
 * @file CIntLiteralExpr.hpp
 * @brief This file contains the definition of the CIntLiteralExpr struct which represents
 * a C integer literal expression construct.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/expr/CExpr.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CIntLiteralExpr
 * @brief Represents a C integer literal expression construct (e.g. `42`, `42u`).
 */
class CIntLiteralExpr : public CExpr {
public:
    basic::APInt value;
    bool isSigned = true;

    CIntLiteralExpr(const basic::APInt& value, bool isSigned = true)
        : value(value), isSigned(isSigned) {}

    virtual ~CIntLiteralExpr() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
