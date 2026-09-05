/**
 * @file CEmptyStmt.hpp
 * @brief This file contains the definition of the CEmptyStmt struct which represents
 * a C empty statement construct (a lone semicolon).
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/stmt/CStmt.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CEmptyStmt
 * @brief Represents a C empty statement construct (i.e. `;`). Useful as a placeholder,
 * e.g. to attach a label to an otherwise empty basic block.
 */
class CEmptyStmt : public CStmt {
public:
    virtual ~CEmptyStmt() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
