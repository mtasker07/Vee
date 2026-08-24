/**
 * @file CLIOptionValidationDelegates.hpp
 * @brief This file contains the definitions of all CLI option validation delegates.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/descriptor/CLIDescriptorFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

// Value forward
class CLIValue;

namespace delegate {

/**
 * @brief Validates values for the input file option.
 * Input files must be valid FILE paths and must exist on disk.
 */
bool inputFileValidationDelegate(descriptor::CLIOptionValidationContext& ctx, const CLIValue& value);
/**
 * @brief Validates values for the output file option.
 * Output files must be valid FILE paths and may or may not exist on disk.
 */
bool outputFileValidationDelegate(descriptor::CLIOptionValidationContext& ctx, const CLIValue& value);
/**
 * @brief Validates values for the optimization level option.
 * Optimization levels must be integers between 0 and 3 (inclusive).
 */
bool optimizationLevelValidationDelegate(descriptor::CLIOptionValidationContext& ctx, const CLIValue& value);
/**
 * @brief Validates values for the MIR output directory option.
 * The output directory must be a valid directory path and may or may not exist on disk.
 */
bool mirOutputDirectoryValidationDelegate(descriptor::CLIOptionValidationContext& ctx, const CLIValue& value);

} // namespace delegate
} // namespace cli
VEEC_NAMESPACE_END
