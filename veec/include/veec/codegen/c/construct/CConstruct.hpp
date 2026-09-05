/**
 * @file CConstruct.hpp
 * @brief This file contains the definition of the CConstruct class which is the base
 * class for all C constructs.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CConstruct
 * @brief Base class for all C constructs.
 */
class CConstruct {
public:
    virtual ~CConstruct() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
