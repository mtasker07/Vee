/**
 * @file CLIInvocation.hpp
 * @brief This file contains the definition of the CLIInvocation class.
 * 
 * The CLIInvocation class represents an invoked command along with its associated options and arguments.
 */

#pragma once

#include <string_view>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/CLICommand.hpp"
#include "veec/cli/CLICommandOptions.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

/**
 * @class CLIInvocation
 * @brief Represents an invoked command along with its associated options and arguments.
 */
class CLIInvocation {
public:
    /**
     * @brief Creates an invalid CLIInvocation (i.e., with an unknown command).
     */
    CLIInvocation() = default;
    /**
     * @brief Creates a CLIInvocation with the specified command and options.
     * @param command The command that was invoked.
     * @param options The command options and arguments associated with the command.
     */
    CLIInvocation(CLICommand command, const CLICommandOptions& options)
        : _command(command), _options(options) {}

    ~CLIInvocation() = default;

    /**
     * @brief Checks if the CLIInvocation is valid (i.e, command is not CLICommand::Unknown).
     * @return True if the CLIInvocation is valid, false otherwise.
     */
    inline bool isValid() const { return _command != CLICommand::Unknown; }

    /**
     * @brief Gets the command that was invoked.
     * @return The command that was invoked.
     */
    CLICommand getCommand() const { return _command; }
    /**
     * @brief Gets the command options and arguments associated with the command.
     * @return The command options and arguments associated with the command.
     */
    const CLICommandOptions& getOptions() const { return _options; }
    
    /**
     * @brief Gets the command path that was used in this invocation.
     * @return A list of strings representing the invocation path.
     */
    inline const std::vector<std::string_view>& getCommandPathUsed() const {
        return _commandPathUsed;
    }
    /**
     * @brief Sets the command path that was used in this invocation.
     * @param path A list of strings representing the invocation path.
     */
    inline void setCommandPathUsed(const std::vector<std::string_view>& path) {
        _commandPathUsed = path;
    }

private:
    CLICommand _command = CLICommand::Unknown;
    CLICommandOptions _options = {};
    std::vector<std::string_view> _commandPathUsed;
};

} // namespace cli
VEEC_NAMESPACE_END
