/**
 * @file CType.hpp
 * @brief This file contains the definition of the CType struct which is the base
 * class for all C type constructs.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/CConstruct.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CType
 * @brief Base class for all C type constructs.
 */
class CType : public CConstruct {
public:
    virtual ~CType() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
