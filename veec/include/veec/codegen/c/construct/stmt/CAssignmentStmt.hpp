/**
 * @file CAssignmentStmt.hpp
 * @brief This file contains the definition of the CAssignmentStmt struct which represents
 * a C assignment statement construct.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/stmt/CStmt.hpp"
#include "veec/codegen/c/construct/expr/CExpr.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

class CAssignmentStmt : public CStmt {
public:
    CExpr* lhs = nullptr;
    CExpr* rhs = nullptr;

    virtual ~CAssignmentStmt() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
