/**
 * @file TypeId.hpp
 * @brief This file contains the definition of the TypeId type.
 *
 * TypeId is a unique identifier for referencing types without
 * directly storing the object itself.
 * All Type objects have a unique TypeId.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace types {

// TypeId -> u32
using TypeId = u32;

} // namespace types
VEEC_NAMESPACE_END
