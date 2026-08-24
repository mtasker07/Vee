/**
 * @file CLIRootDescriptor.hpp
 * @brief This file contains the definition of the CLIRootDescriptor class and related types.
 * 
 * The CLIRootDescriptor is the descriptor that contains all the other descriptors for the CLI.
 * It also contains global options (options that work with any command).
 * 
 * You can think of this like the command descriptor for the veec.exe command.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/descriptor/CLICommandDescriptor.hpp"
#include "veec/cli/descriptor/CLIOptionDescriptor.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace descriptor {

/**
 * @struct CLIRootDescriptor
 * @brief The root descriptor that contains all commands and global options in the CLI.
 */
struct CLIRootDescriptor {
    /**
     * @brief The index of the command to execute by default if no command is specified.
     * This should usually be the index of the 'compile' command.
     */
    size_t defaultCommandIndex = 0;
    /**
     * @brief The list of all root commands in the CLI.
     */
    std::vector<CLICommandDescriptor> commands;
    /**
     * @brief The list of all global options in the CLI. Global options can be used within any
     * command context, for example "--verbose".
     */
    std::vector<CLIOptionDescriptor> globalOptions;
};

/**
 * @brief Gets the root descriptor for the CLI. This is a singleton that contains ALL commands
 * and global options in the CLI.
 */
const descriptor::CLIRootDescriptor& getRootDescriptor();

} // namespace descriptor
} // namespace cli
VEEC_NAMESPACE_END
