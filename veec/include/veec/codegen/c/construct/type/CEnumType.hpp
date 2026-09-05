/**
 * @file CEnumType.hpp
 * @brief This file contains the definition of the CEnumType struct which represents
 * a C enum type construct.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/type/CType.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CEnumType
 * @brief Represents a C enum type construct.
 */
class CEnumType : public CType {
public:
    CType* underlyingType;
    std::vector<CType*> enumerators;
    
    virtual ~CEnumType() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
