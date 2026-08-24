/**
 * @file CLIArgsParseResult.hpp
 * @brief This file contains the definition of the CLIArgsParseResult class.
 * 
 * The CLIArgsParseResult class stores data about parsed cli options.
 */

#pragma once

#include <string_view>
#include <vector>
#include <cstddef>
#include <utility>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/CLICommand.hpp"
#include "veec/cli/CLIOption.hpp"
#include "veec/cli/CLIValue.hpp"
#include "veec/cli/CLIRawArgs.hpp"
#include "veec/cli/CLIArgsToken.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace parsing {

class CLIArgsParsedOption {
public:
    CLIOption option = CLIOption::Unknown;
    std::string_view nameUsed;
    // ^^ May be long or short name, use for diagnostics
    size_t argIndex = 0;
    std::vector<CLIValue> values;
}; 

class CLIArgsParseResult {
public:
    CLIArgsParseResult() = default;
    ~CLIArgsParseResult() = default;

    /**
     * @brief Gets the command for this parse result.
     * @return The command for this parse result.
     */
    CLICommand getCommand() const {
        return _command;
    }
    /**
     * @brief Sets the command for this parse result.
     * @param command The command to set.
     */
    void setCommand(CLICommand command) {
        _command = command;
    }

    //
    // Positionals
    //

    inline const std::vector<std::string_view>& getPositionals() const {
        return _positionals;
    }
    /**
     * @brief Adds a positional argument.
     * @param positional The positional argument to add.
     */
    inline void addPositional(std::string_view positional) {
        _positionals.push_back(positional);
    }

    //
    // Options
    //

    inline const std::vector<CLIArgsParsedOption>& getOptions() const {
        return _options;
    }
    inline void addOption(CLIOption option, std::string_view nameUsed, size_t argIndex, std::vector<CLIValue> values = {}) {
        CLIArgsParsedOption parsedOption;
        parsedOption.option = option;
        parsedOption.argIndex = argIndex;
        parsedOption.nameUsed = nameUsed;
        parsedOption.values = std::move(values);
        _options.push_back(std::move(parsedOption));
    }

private:
    CLICommand _command = CLICommand::Unknown;
    std::vector<std::string_view> _commandPathUsed;
    std::vector<std::string_view> _positionals;
    std::vector<CLIArgsParsedOption> _options;
};

} // namespace parsing
} // namespace cli
VEEC_NAMESPACE_END
