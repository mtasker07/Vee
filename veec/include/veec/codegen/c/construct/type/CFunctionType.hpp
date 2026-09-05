/**
 * @file CFunctionType.hpp
 * @brief This file contains the definition of the CFunctionType struct which represents
 * a C function type construct.
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
 * @class CFunctionType
 * @brief Represents a C function type construct.
 */
class CFunctionType : public CType {
public:
    CType* returnType;
    std::vector<CType*> paramTypes;
    
    virtual ~CFunctionType() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
