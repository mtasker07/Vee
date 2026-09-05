/**
 * @file CTypedefType.hpp
 * @brief This file contains the definition of the CTypedefType struct which represents
 * a C typedef type construct.
 */

#pragma once

#include <vector>
#include <string>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/type/CType.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CTypedefType
 * @brief Represents a C typedef type construct.
 */
class CTypedefType : public CType {
public:
    CType* underlyingType;
    std::string name;
    
    virtual ~CTypedefType() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
