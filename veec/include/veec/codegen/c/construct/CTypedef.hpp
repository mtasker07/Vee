/**
 * @file CTypedef.hpp
 * @brief This file contains the definition of the CTypedef struct which represents
 * a C typedef construct.
 */

#pragma once

#include <string>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/CConstruct.hpp"
#include "veec/codegen/c/construct/type/CType.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CTypedef
 * @brief Represents a C typedef construct.
 */
class CTypedef : public CConstruct {
public:
    CType* type;
    std::string name;
    
    virtual ~CTypedef() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
