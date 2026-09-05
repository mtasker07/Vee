/**
 * @file CLabelStmt.hpp
 * @brief This file contains the definition of the CLabelStmt struct which represents
 * a C labeled statement construct.
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
 * @class CLabelStmt
 * @brief Represents a C labeled statement construct (e.g. `L0: <stmt>`).
 * @note In C, a label must be attached to a statement. Use CEmptyStmt for the wrapped
 * statement if the label has no meaningful statement to attach to (e.g. an empty basic block).
 */
class CLabelStmt : public CStmt {
public:
    std::string label;
    CStmt* stmt = nullptr;

    virtual ~CLabelStmt() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
