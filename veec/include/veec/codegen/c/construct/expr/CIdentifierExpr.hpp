/**
 * @file CIdentifierExpr.hpp
 * @brief This file contains the definition of the CIdentifierExpr struct which represents
 * a reference to a named C entity (variable, parameter, function, etc.) used as an expression.
 */

#pragma once

#include <string>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/expr/CExpr.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CIdentifierExpr
 * @brief Represents a reference to a named C entity used as an expression (e.g. a variable name).
 */
class CIdentifierExpr : public CExpr {
public:
    std::string name;

    virtual ~CIdentifierExpr() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
