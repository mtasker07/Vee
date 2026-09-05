/**
 * @file CCastExpr.hpp
 * @brief This file contains the definition of the CCastExpr struct which represents
 * a C explicit cast expression construct.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/expr/CExpr.hpp"
#include "veec/codegen/c/construct/type/CType.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CCastExpr
 * @brief Represents a C explicit cast expression construct (e.g. `(int)x`).
 */
class CCastExpr : public CExpr {
public:
    CType* targetType = nullptr;
    CExpr* operand = nullptr;

    virtual ~CCastExpr() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
