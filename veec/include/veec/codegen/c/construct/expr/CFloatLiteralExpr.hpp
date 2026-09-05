/**
 * @file CFloatLiteralExpr.hpp
 * @brief This file contains the definition of the CFloatLiteralExpr struct which represents
 * a C floating-point literal expression construct.
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
 * @class CFloatLiteralExpr
 * @brief Represents a C floating-point literal expression construct (e.g. `1.0f`, `1.0`).
 */
class CFloatLiteralExpr : public CExpr {
public:
    double value = 0.0;
    /// @brief Whether this literal is double precision (`double`) as opposed to single precision (`float`).
    bool isDouble = true;

    CFloatLiteralExpr(double value = 0.0, bool isDouble = true)
        : value(value), isDouble(isDouble) {}

    virtual ~CFloatLiteralExpr() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
