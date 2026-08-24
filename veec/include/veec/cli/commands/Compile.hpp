/**
 * @file Compile.hpp
 * @brief This file contains the declaration of the compile command.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/descriptor/CLIDescriptorFwd.hpp"
#include "veec/cli/delegate/CLIDelegateTypes.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace commands {

/**
 * @brief Gets the option descriptors for the compile command.
 * @return A list of option descriptors for the compile command.
 */
const std::vector<descriptor::CLIOptionDescriptor>& getCompileCommandOptionDescriptors();
/**
 * @brief Gets the descriptor for the compile command.
 * @return A reference to the CLICommandDescriptor for the compile command.
 */
const descriptor::CLICommandDescriptor& getCompileCommandDescriptor();
/**
 * @brief Gets the invocation delegate for the compile command.
 * @return A reference to the CLICommandInvocationDelegate for the compile command.
 */
const delegate::CLICommandInvocationDelegate& getCompileCommandInvocationDelegate();

} // namespace commands
} // namespace cli
VEEC_NAMESPACE_END
