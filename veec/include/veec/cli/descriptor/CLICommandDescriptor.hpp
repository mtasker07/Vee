/**
 * @file CLICommandDescriptor.hpp
 * @brief This file contains the definition of all types related to cli commands.
 */

#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/CLICommand.hpp"
#include "veec/cli/delegate/CLIDelegateTypes.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace descriptor {

// Forward declare descriptors
struct CLIOptionDescriptor;
struct CLICommandDescriptor;

/**
 * @struct CLICommandInvocationContext
 * @brief Holds context for invoking a CLI command.
 */
struct CLICommandInvocationContext {
    const CLICommandDescriptor& descriptor;
};

/**
 * @struct CLICommandDescriptor
 * @brief Describes a command-line command, including its name, description, options, and subcommands.
 */
struct CLICommandDescriptor {
    /**
     * @brief The command represented by this descriptor.
     */
    CLICommand command = CLICommand::Unknown;
    /**
     * @brief A list of all subcommands for this command. If empty, this command is considered
     * a leaf command.
     */
    std::vector<CLICommandDescriptor> subcommands;
    /**
     * @brief The primary name of the command. This is the main name that can be used to invoke
     * the command and will be displayed in most user-facing messages.
     */
    std::string_view namePrimary;
    /**
     * @brief An optional list of aliases for the command. These are alternatives names that can
     * be used to invoke the command but are generally not displayed in user-facing messages.
     */
    std::vector<std::string_view> nameAliases;
    /**
     * @brief A brief description of the command. This is used for generating help messages.
     */
    std::string_view description;
    /**
     * @brief A list of option descriptors that are valid for this command.
     */
    std::vector<CLIOptionDescriptor> options;
    /**
     * @brief An optional delegate that is called when the command is invoked. This is only called
     * for leaf commands (commands with no subcommands).
     */
    delegate::CLICommandInvocationDelegate invocationDelegate = nullptr;
};

//
// CommandDescriptor utilities
//

/**
 * @brief Gets the command descriptor for a given CLICommand.
 * @param command The CLICommand to get the descriptor for.
 * @return A pointer to the command descriptor for the given command,
 * or nullptr if there isn't one defined.
 */
const CLICommandDescriptor* getCommandDescriptor(CLICommand command);
/**
 * @brief Gets the command descriptor for a given command name.
 * @param name The name of the command to get the descriptor for.
 * @param parent The parent command descriptor to search under. If nullptr, search is performed on the root
 * descriptor's command list.
 * @param includeAliases Whether to include aliases in the search. If true, the search will also check
 * the aliases of each command descriptor. If false, only the primary names will be checked.
 * @return A pointer to the command descriptor for the given name, or nullptr if there isn't one defined.
 */
const CLICommandDescriptor* getCommandDescriptorByName(
    std::string_view name,
    const CLICommandDescriptor* parent = nullptr,
    bool includeAliases = true
);

} // namespace descriptor
} // namespace cli
VEEC_NAMESPACE_END
