/**
 * @file CLIOptions.hpp
 * @brief This file contains the definition of the CLIOptions class.
 * 
 * The CLIOptions class stores data about parsed cli options.
 */

#pragma once

#include <string_view>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/CLIOption.hpp"
#include "veec/cli/CLIValue.hpp"
#include "veec/cli/CLIArgsToken.hpp"
#include "veec/cli/descriptor/CLIOptionDescriptor.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

class CLICommandOptions {
public:
    CLICommandOptions() = default;
    ~CLICommandOptions() = default;

    //
    // Positionals
    //

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

    /**
     * @brief Checks if the given option was specified in the command-line arguments.
     * @param option The option to check.
     * @return True if the option was specified, false otherwise.
     */
    inline bool isOptionSpecified(CLIOption option) const {
        return _specifiedOptions.find(option) != _specifiedOptions.end();
    }

    //
    // Flags
    //

    /**
     * @brief Checks if a flag option is specified.
     * @param option The flag option to check. Must be a flag option.
     * @return True if the flag option is specified, false otherwise.
     */
    inline bool isFlagSpecified(CLIOption option) const {
        VEE_ASSERT(isFlagOption(option), "Option is not a flag option");
        return isOptionSpecified(option);
    }
    /**
     * @brief Sets a flag option as specified. Asserts if set more than once for
     * a given option, so that should be checked beforehand.
     * @param option The flag option to set. Must be a flag option.
     */
    inline void setFlag(CLIOption option) {
        VEE_ASSERT(isFlagOption(option), "Option is not a flag option");
        VEE_ASSERT(!isOptionSpecified(option), "Flag option already specified");
        _specifiedOptions.insert(option);
    }

    //
    // Counters
    //

    /**
     * @brief Gets the count of how many times a counter option was specified.
     * @param option The counter option to get the count for. Must be a counter option.
     * @return The count of how many times the counter option was specified.
     */
    inline u32 getCounterValue(CLIOption option) const {
        VEE_ASSERT(isCounterOption(option), "Option is not a counter option");

        auto it = _counterOptions.find(option);
        if (it != _counterOptions.end()) {
            return it->second;
        }
        return 0;
    }
    /**
     * @brief Increments the count of how many times a counter option was specified.
     * @param option The counter option to increment the count for. Must be a counter option.
     */
    inline void incrementCounter(CLIOption option) {
        VEE_ASSERT(isCounterOption(option), "Option is not a counter option");
        _specifiedOptions.insert(option);
        ++_counterOptions[option];
    }

    //
    // Values
    //

    /**
     * @brief Gets the value of a value option.
     * @param option The value option to get the value for. Must be a value option.
     * @return The value of the value option, or nullptr if not specified.
     */
    inline const CLIValue* getValue(CLIOption option) const {
        VEE_ASSERT(isValueOption(option), "Option is not a value option");

        auto it = _valueOptions.find(option);
        if (it != _valueOptions.end()) {
            return &it->second;
        }
        return nullptr;
    }
    /**
     * @brief Gets the value of a value option or a default value if not specified.
     * @param option The value option to get the value for. Must be a value option.
     * @return The value of the value option, or defaultValue if not specified.
     */
    inline CLIValue getValueOr(CLIOption option, CLIValue defaultValue) const {
        VEE_ASSERT(isValueOption(option), "Option is not a value option");

        auto it = _valueOptions.find(option);
        if (it != _valueOptions.end()) {
            return it->second;
        }
        return defaultValue;
    }
    /**
     * @brief Sets the value of a value option. Asserts if set more than once for
     * a given option, so that should be checked beforehand.
     * @param option The value option to set the value for. Must be a value option.
     * @param value The value to set for the option.
     */
    inline void setValue(CLIOption option, CLIValue value) {
        VEE_ASSERT(isValueOption(option), "Option is not a value option");
        VEE_ASSERT(!isOptionSpecified(option), "Value option already specified");
        _specifiedOptions.insert(option);
        _valueOptions[option] = value;
    }
    
    //
    // Lists
    //

    /**
     * @brief Gets the list of values for a list option.
     * @param option The list option to get the values for. Must be a list option.
     * @return The list of values for the list option, or an empty vector
     * if not specified.
     */
    inline const std::vector<CLIValue>& getListValues(CLIOption option) const {
        VEE_ASSERT(isListOption(option), "Option is not a list option");

        auto it = _listOptions.find(option);
        if (it != _listOptions.end()) {
            return it->second;
        }
        static const std::vector<CLIValue> empty;
        return empty;
    }
    /**
     * @brief Adds an empty list for a list option. This is used to indicate that the
     * list option was specified but no values were provided. Asserts if set more than once for
     * a given option, so that should be checked beforehand.
     * @param option The list option to add an empty list for. Must be a list option.
     */
    inline void addEmptyList(CLIOption option) {
        VEE_ASSERT(isListOption(option), "Option is not a list option");
        _specifiedOptions.insert(option);
    }
    /**
     * @brief Adds a value to a list option.
     * @param option The list option to add the value to. Must be a list option.
     * @param value The value to add to the list option. Must be convertable to
     * the list option's value type.
     */
    inline void addListValue(CLIOption option, CLIValue value) {
        VEE_ASSERT(isListOption(option), "Option is not a list option");
        VEE_ASSERT(value.canBe(descriptor::getOptionDescriptor(option)->valueType),
            "Value cannot be converted to list option's value type");

        _specifiedOptions.insert(option);
        _listOptions[option].push_back(value);
    }
    inline void addListValues(CLIOption option, const std::vector<CLIValue>& values) {
        VEE_ASSERT(isListOption(option), "Option is not a list option");
        for (const CLIValue& value : values) {
            VEE_ASSERT(value.canBe(descriptor::getOptionDescriptor(option)->valueType),
                "Value cannot be converted to list option's value type");
        }

        _specifiedOptions.insert(option);
        _listOptions[option].insert(_listOptions[option].end(), values.begin(), values.end());
    }

private:
    std::vector<std::string_view> _positionals;
    std::unordered_set<CLIOption> _specifiedOptions;
    std::unordered_map<CLIOption, CLIValue> _valueOptions;
    std::unordered_map<CLIOption, std::vector<CLIValue>> _listOptions;
    std::unordered_map<CLIOption, u32> _counterOptions;
};

} // namespace cli
VEEC_NAMESPACE_END
