/**
 * @file CBoolLiteralExpr.hpp
 * @brief This file contains the definition of the CBoolLiteralExpr struct which represents
 * a C boolean literal expression construct.
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
 * @class CBoolLiteralExpr
 * @brief Represents a C boolean literal expression construct (e.g. `true`, `false`).
 */
class CBoolLiteralExpr : public CExpr {
public:
    bool value = false;

    CBoolLiteralExpr(bool value = false)
        : value(value) {}

    virtual ~CBoolLiteralExpr() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
