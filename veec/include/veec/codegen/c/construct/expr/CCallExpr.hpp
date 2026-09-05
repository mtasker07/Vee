/**
 * @file CCallExpr.hpp
 * @brief This file contains the definition of the CCallExpr struct which represents
 * a C function call used as an expression (i.e. producing a value).
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/expr/CExpr.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CCallExpr
 * @brief Represents a C function call expression construct (e.g. `foo(a, b)`).
 */
class CCallExpr : public CExpr {
public:
    CExpr* callee = nullptr;
    std::vector<CExpr*> args;

    virtual ~CCallExpr() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
