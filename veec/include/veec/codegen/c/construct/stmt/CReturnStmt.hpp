/**
 * @file CReturnStmt.hpp
 * @brief This file contains the definition of the CReturnStmt struct which represents
 * a C return statement construct.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/stmt/CStmt.hpp"
#include "veec/codegen/c/construct/expr/CExpr.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CReturnStmt
 * @brief Represents a C return statement construct (e.g. `return x;` or `return;`).
 */
class CReturnStmt : public CStmt {
public:
    /// @brief The value to return, or nullptr for a void return.
    CExpr* value = nullptr;

    virtual ~CReturnStmt() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
