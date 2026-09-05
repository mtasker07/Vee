/**
 * @file CCallStmt.hpp
 * @brief This file contains the definition of the CCallStmt struct which represents
 * a C call statement construct.
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

class CCallStmt : public CStmt {
public:
    CExpr* function = nullptr;
    std::vector<CExpr*> arguments;

    virtual ~CCallStmt() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
