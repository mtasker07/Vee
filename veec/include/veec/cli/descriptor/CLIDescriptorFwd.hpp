/**
 * @file CLIDescriptorFwd.hpp
 * @brief This file contains the forward declarations of all CLI descriptor types.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace descriptor {

// CLIRootDescriptor.hpp
struct CLIRootDescriptor;

// CLICommandDescriptor.hpp
struct CLICommandDescriptor;
struct CLICommandInvocationContext;

// CLIOptionDescriptor.hpp
struct CLIOptionDescriptor;
struct CLIOptionValidationContext;

} // namespace descriptor
} // namespace cli
VEEC_NAMESPACE_END
