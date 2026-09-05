/**
 * @file CDeclStmt.hpp
 * @brief This file contains the definition of the CDeclStmt struct which represents
 * a C local variable declaration statement construct.
 */

#pragma once

#include <string>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/stmt/CStmt.hpp"
#include "veec/codegen/c/construct/expr/CExpr.hpp"
#include "veec/codegen/c/construct/type/CType.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CDeclStmt
 * @brief Represents a C local variable declaration statement construct (e.g. `int x = 1;`).
 */
class CDeclStmt : public CStmt {
public:
    CType* type = nullptr;
    std::string name;
    /// @brief Optional initializer expression. May be nullptr for an uninitialized declaration.
    CExpr* init = nullptr;

    virtual ~CDeclStmt() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
