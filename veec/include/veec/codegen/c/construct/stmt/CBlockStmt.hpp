/**
 * @file CBlockStmt.hpp
 * @brief This file contains the definition of the CBlockStmt struct which represents
 * a C block statement construct.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/stmt/CStmt.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

class CBlockStmt : public CStmt {
public:
    std::vector<CStmt*> statements;

    virtual ~CBlockStmt() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
