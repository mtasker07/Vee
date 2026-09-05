/**
 * @file CCompoundLiteralExpr.hpp
 * @brief This file contains the definition of the CCompoundLiteralExpr struct which represents
 * a C compound literal expression construct, used to construct aggregate (struct) values inline.
 */

#pragma once

#include <vector>

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
 * @class CCompoundLiteralExpr
 * @brief Represents a C compound literal expression construct (e.g. `(struct Foo){ a, b }`).
 */
class CCompoundLiteralExpr : public CExpr {
public:
    CType* type = nullptr;
    std::vector<CExpr*> values;

    virtual ~CCompoundLiteralExpr() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
