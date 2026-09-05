/**
 * @file CConstant.hpp
 * @brief This file contains the definition of the CConstant struct which represents
 * a C constant construct.
 */

#pragma once

#include <string>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/CConstruct.hpp"
#include "veec/codegen/c/construct/expr/CExpr.hpp"
#include "veec/codegen/c/construct/type/CType.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CConstant
 * @brief Represents a C constant construct.
 */
class CConstant : public CConstruct {
public:
    std::string name;
    CExpr* value;
    
    virtual ~CConstant() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
