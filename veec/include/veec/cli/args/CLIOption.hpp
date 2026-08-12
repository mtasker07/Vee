/**
 * @file CLIOption.hpp
 * @brief This file contains the definition of all types related to command-line options.
 */

#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <sstream>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/args/CLIValue.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace args {

/**
 * @enum CLIOption
 * @brief Represents the different command-line options that can be specified.
 * Unknown is a special option that is used when an unrecognized option is encountered by the parser.
 * 
 * All CLIOptions should have a corresponding CLIOptionDescriptor in CLI_OPTION_DESCRIPTORS,
 * except for Unknown, which is a special sentinel.
 */
enum class CLIOption : u8 {
    Unknown, // Unknown option
    Help,
    Version,
    InputFile,
    OutputFile,
    OptimizationLevel,
    OutputMir,
};

/**
 * @enum CLIOptionType
 * @brief Represents the type of a command-line option.
 */
enum class CLIOptionType : u8 {
    /**
     * @brief Flags are options that do not take any values and can only be specified once.
     * They are typically used to enable or disable a feature. and theyre behaviour is always
     * simply negating the default option's value when specified.
     */
    Flag,
    /**
     * @brief Counters are flag-like options that do not take any values but can be specified more
     * than once. The number of times the option is specified is counted and stored as a value.
     */
    Counter,
    /**
     * @brief Value options are options that take a single value. They can only be specified once.
     */
    Value,
    List, // Option can be specified multiple times and collects required values into a list
};

struct CLIOptionDescriptor {
    /**
     * @brief The option represented by this descriptor.
     */
    CLIOption option = CLIOption::Unknown;
    /**
     * @brief Short name of the option. (e.g. 'h' for 'help') (optional).
     */
    char nameShort = '\0';
    /**
     * @brief Long name of the option. (e.g. 'help').
     */
    std::string_view nameLong;
    /**
     * @brief Brief description of the option. This is used for generating help messages.
     */
    std::string_view description;
    /**
     * @brief Type of the option (flag, counter, value, or list).
     */
    CLIOptionType type;
    /**
     * @brief Type of the value(s) expected for this option. For flags and counters,
     * this value is ignored.
     */
    CLIValueType valueType = CLIValueType::None;
    /**
     * @brief The default value(s) for the option. For counters, this value is ignored.
     * For flags, use flagDefaultValue instead and leave this empty. For lists, this
     * value is ignored and the list is always empty by default.
     */
    CLIValue defaultValue = {};
    /**
     * @brief The default value for flags. For other option types, this value is ignored.
     */
    bool flagDefaultValue = false;
    /**
     * @brief Minimum number of values required for the option.
     * For values, flags and counters, this value is ignored.
     */
    i32 minValues = 0;
    /**
     * @brief Maximum number of values allowed for the option.
     * For values and flags, this value is ignored.
     * For counters, this value is the maximum number of times the option can be specified.
     * A value of -1 indicates no maximum limit.
     */
    i32 maxValues = 0;
    /**
     * @brief Whether this option is required or optional. If true, the parser will report an
     * error if the option is not specified.
     */
    bool required = false;
};

/**
 * @brief A static array that stores all the CLI option descriptors. To add additional command-line options, add them to this array
 * and they will instantly be recognised by the parser.
 */
static constexpr CLIOptionDescriptor CLI_OPTION_DESCRIPTORS[] = {

    // HELP

    {
        /* option               */ CLIOption::Help,
        /* nameShort            */ 'h',
        /* nameLong             */ "help",
        /* description          */ "Display help information",
        /* type                 */ CLIOptionType::Flag,
        /* valueType            */ CLIValueType::None,
        /* defaultValue         */ {},
        /* flagDefaultValue     */ false,
        /* minValues            */ 0,
        /* maxValues            */ 1,
        /* required             */ false,
    },

    // VERSION

    {
        /* option               */ CLIOption::Version,
        /* nameShort            */ 'v',
        /* nameLong             */ "version",
        /* description          */ "Display version information",
        /* type                 */ CLIOptionType::Flag,
        /* valueType            */ CLIValueType::None,
        /* defaultValue         */ {},
        /* flagDefaultValue     */ false,
        /* minValues            */ 0,
        /* maxValues            */ 1,
        /* required             */ false,
    },

    // INPUT FILE(S)

    {
        /* option               */ CLIOption::InputFile,
        /* nameShort            */ 'i',
        /* nameLong             */ "input",
        /* description          */ "Input file(s)",
        /* type                 */ CLIOptionType::List,
        /* valueType            */ CLIValueType::String,
        /* defaultValue         */ {},
        /* flagDefaultValue     */ false,
        /* minValues            */ 1,
        /* maxValues            */ -1,
        /* required             */ true,
    },

    // OUTPUT FILE

    {
        /* option               */ CLIOption::OutputFile,
        /* nameShort            */ 'o',
        /* nameLong             */ "output-file",
        /* description          */ "Output file",
        /* type                 */ CLIOptionType::Value,
        /* valueType            */ CLIValueType::String,
        /* defaultValue         */ {},
        /* flagDefaultValue     */ false,
        /* minValues            */ 1,
        /* maxValues            */ 1,
        /* required             */ true,
    },

    // OPTIMIZATION LEVEL

    {
        /* option               */ CLIOption::OptimizationLevel,
        /* nameShort            */ 'O',
        /* nameLong             */ "opt-level",
        /* description          */ "Optimization level",
        /* type                 */ CLIOptionType::Value,
        /* valueType            */ CLIValueType::Integer,
        /* defaultValue         */ {},
        /* flagDefaultValue     */ false,
        /* minValues            */ 1,
        /* maxValues            */ 1,
        /* required             */ false,
    },

    // OUTPUT MIR

    {
        /* option               */ CLIOption::OutputMir,
        /* nameShort            */ '\0',
        /* nameLong             */ "output-mir",
        /* description          */ "Output MIR to directory",
        /* type                 */ CLIOptionType::Value,
        /* valueType            */ CLIValueType::String,
        /* defaultValue         */ {},
        /* flagDefaultValue     */ false,
        /* minValues            */ 1,
        /* maxValues            */ 1,
        /* required             */ false,
    },
};
/**
 * @brief The number of option descriptors in the array.
 */
static constexpr size_t CLI_OPTION_DESCRIPTOR_COUNT = sizeof(CLI_OPTION_DESCRIPTORS) / sizeof(CLI_OPTION_DESCRIPTORS[0]);

//
// Option utilities
//

/**
 * @brief Gets the option descriptor for a given CLIOption.
 * @param option The CLIOption to get the descriptor for.
 * @return A pointer to the option descriptor for the given option,
 * or nullptr if there isn't one defined.
 */
inline const CLIOptionDescriptor* getOptionDescriptor(CLIOption option) {
    for (size_t i = 0; i < CLI_OPTION_DESCRIPTOR_COUNT; ++i) {
        if (CLI_OPTION_DESCRIPTORS[i].option == option) {
            return &CLI_OPTION_DESCRIPTORS[i];
        }
    }
    return nullptr;
}

/**
 * @brief Gets the full/pretty name of a CLI option. This will be in the format:
 * "--long-name" or "--long-name (-s)" if a short name is defined.
 * @param option The CLIOption to get the full name for.
 * @return A string containing the full name of the option.
 */
inline std::string getOptionFullName(CLIOption option) {
    const CLIOptionDescriptor* descriptor = getOptionDescriptor(option);
    if (!descriptor) {
        return "<unknown>";
    }
    std::ostringstream oss;
    oss << "--" << descriptor->nameLong;
    if (descriptor->nameShort != '\0') {
        oss << " (-" << descriptor->nameShort << ")";
    }
    return oss.str();
}

/**
 * @brief Checks if a given CLIOption is a flag option.
 * @param option The CLIOption to check.
 * @return True if the option is a flag option, false otherwise.
 */
inline bool isFlagOption(CLIOption option) {
    const CLIOptionDescriptor* descriptor = getOptionDescriptor(option);
    return descriptor && descriptor->type == CLIOptionType::Flag;
}
/**
 * @brief Checks if a given CLIOption is a counter option.
 * @param option The CLIOption to check.
 * @return True if the option is a counter option, false otherwise.
 */
inline bool isCounterOption(CLIOption option) {
    const CLIOptionDescriptor* descriptor = getOptionDescriptor(option);
    return descriptor && descriptor->type == CLIOptionType::Counter;
}
/**
 * @brief Checks if a given CLIOption is a value option.
 * @param option The CLIOption to check.
 * @return True if the option is a value option, false otherwise.
 */
inline bool isValueOption(CLIOption option) {
    const CLIOptionDescriptor* descriptor = getOptionDescriptor(option);
    return descriptor && descriptor->type == CLIOptionType::Value;
}
/**
 * @brief Checks if a given CLIOption is a list option.
 * @param option The CLIOption to check.
 */
inline bool isListOption(CLIOption option) {
    const CLIOptionDescriptor* descriptor = getOptionDescriptor(option);
    return descriptor && descriptor->type == CLIOptionType::List;
}

} // namespace args
} // namespace cli
VEEC_NAMESPACE_END
