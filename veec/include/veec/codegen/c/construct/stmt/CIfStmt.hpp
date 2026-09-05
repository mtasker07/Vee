/**
 * @file CIfStmt.hpp
 * @brief This file contains the definition of the CIfStmt struct which represents
 * a C if statement construct.
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
 * @class CIfStmt
 * @brief Represents a C if statement construct (e.g. `if (cond) thenStmt; else elseStmt;`).
 */
class CIfStmt : public CStmt {
public:
    CExpr* condition = nullptr;
    CStmt* thenStmt = nullptr;
    /// @brief Optional else branch. May be nullptr if there is none.
    CStmt* elseStmt = nullptr;

    virtual ~CIfStmt() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
