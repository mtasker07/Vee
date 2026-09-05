/**
 * @file CGotoStmt.hpp
 * @brief This file contains the definition of the CGotoStmt struct which represents
 * a C goto statement construct.
 */

#pragma once

#include <string>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/stmt/CStmt.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CGotoStmt
 * @brief Represents a C goto statement construct (e.g. `goto L0;`).
 */
class CGotoStmt : public CStmt {
public:
    std::string label;

    virtual ~CGotoStmt() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
