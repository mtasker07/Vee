/**
 * @file CLIDelegateTypes.hpp
 * @brief This file contains the definitions of ALL CLI delegate types.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/descriptor/CLIDescriptorFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

// Forwards
class CLIValue;
class CLICommandOptions;

namespace delegate {

/**
 * @brief The CLIOptionValidationDelegate is a delegate responsible for validating CLI option values.
 * It takes a CLIOptionValidationContext and a CLIValue as parameters and returns a boolean indicating
 * whether the value is valid or not.
 */
typedef bool (*CLIOptionValidationDelegate)(descriptor::CLIOptionValidationContext& ctx, const CLIValue& value);

/**
 * @brief The CLICommandInvocationDelegate is a delegate responsible for handling the invocation of CLI
 * commands. It takes a CLICommandInvocationContext as a parameter and returns an integer status code.
 */
typedef int (*CLICommandInvocationDelegate)(descriptor::CLICommandInvocationContext& ctx, const CLICommandOptions& options);

} // namespace delegate
} // namespace cli
VEEC_NAMESPACE_END
