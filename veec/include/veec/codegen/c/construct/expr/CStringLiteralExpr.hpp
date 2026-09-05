/**
 * @file CStringLiteralExpr.hpp
 * @brief This file contains the definition of the CStringLiteralExpr struct which represents
 * a C string literal expression construct.
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
 * @class CStringLiteralExpr
 * @brief Represents a C string literal expression construct (e.g. `"hello"`).
 * @note The value is stored unescaped; escaping is the responsibility of the printer.
 */
class CStringLiteralExpr : public CExpr {
public:
    std::string value;

    CStringLiteralExpr(const std::string& value = "")
        : value(value) {}

    virtual ~CStringLiteralExpr() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
