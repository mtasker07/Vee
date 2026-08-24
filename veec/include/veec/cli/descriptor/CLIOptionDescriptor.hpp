/**
 * @file CLIOptionDescriptor.hpp
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
#include "veec/cli/CLICommand.hpp"
#include "veec/cli/CLIOption.hpp"
#include "veec/cli/CLIOptionType.hpp"
#include "veec/cli/CLIValue.hpp"

VEEC_NAMESPACE_BEGIN

namespace diagnostics {
    class DiagnosticEngine;
    class DiagnosticRange;
}

namespace cli {
namespace descriptor {

// vv For use in CLIOptionValidationContext
struct CLIOptionDescriptor;

/**
 * @struct CLIOptionValidationContext
 * @brief Holds context for validating a CLI option.
 */
struct CLIOptionValidationContext {
    /// @brief Diagnostic engine for reporting diagnostics.
    diagnostics::DiagnosticEngine& diagnostics;
    /// @brief The range of the value being validated (mostly for report(...)).
    const diagnostics::DiagnosticRange& valueRange;
	/// @brief The descriptor of the option being validated.
	const CLIOptionDescriptor& descriptor;
};

/**
 * @struct CLIOptionDescriptor
 * @brief Describes a command-line option, including its name, type, description, and validation rules.
 */
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
    /**
     * @brief Optional callback for additional validation of a single value.
     *
     * For list options, this is intended to be called once per element.
     * Report necessary diagnostics through the engine, and return true/false to indicate
     * whether the value is valid or not. The caller (validator) won't report any
     * diagnostics if validation fails, so you should never blindly return false.
     * 
     * This is intended for more complex validation that cannot be expressed through the descriptor.
     * For example, dont bother checking things like value type, as the validator does that already
     * through the descriptor's valueType field.
     * 
	 * @return True if the value is valid, false otherwise.
     */
    bool (*validateValue)(CLIOptionValidationContext& ctx, const CLIValue& value) = nullptr;
};

//
// Option utilities
//

/**
 * @brief Gets the option descriptor for a given CLIOption.
 * @param option The CLIOption to get the descriptor for.
 * @return A pointer to the option descriptor for the given option,
 * or nullptr if there isn't one defined.
 */
const CLIOptionDescriptor* getOptionDescriptor(CLIOption option);
/**
 * @brief Gets the option descriptor for a given CLIOption by its long name.
 * @param command The command to search for the option in. Pass CLICommand::Unknown to query global options.
 * @param longName The long name of the option to get the descriptor for.
 * @return A pointer to the option descriptor for the given long name, or nullptr if there isn't one defined.
 */
const CLIOptionDescriptor* getOptionDescriptorByLongName(CLICommand command, std::string_view longName);
/**
 * @brief Gets the option descriptor for a given CLIOption by its short name.
 * @param command The command to search for the option in. Pass CLICommand::Unknown to query global options.
 * @param shortName The short name of the option to get the descriptor for.
 * @return A pointer to the option descriptor for the given short name, or nullptr if there isn't one defined.
 */
const CLIOptionDescriptor* getOptionDescriptorByShortName(CLICommand command, char shortName);

/**
 * @brief Gets the full/pretty name of a CLI option. This will be in the format:
 * "--long-name" or "--long-name (-s)" if a short name is defined.
 * @param option The CLIOption to get the full name for.
 * @return A string containing the full name of the option.
 */
inline std::string getOptionFullName(const CLIOptionDescriptor& optionDescriptor) {
    if (optionDescriptor.nameLong.empty()) {
        return "<unknown>";
    }
    std::ostringstream oss;
    oss << "--" << optionDescriptor.nameLong;
    if (optionDescriptor.nameShort != '\0') {
        oss << " (-" << optionDescriptor.nameShort << ")";
    }
    return oss.str();
}

/**
 * @brief Checks if a given CLIOptionDescriptor corresponds to a flag option.
 * @param optionDescriptor The CLIOptionDescriptor to check.
 * @return True if the CLIOptionDescriptor corresponds to a flag option, false otherwise.
 */
inline bool isFlagOption(const CLIOptionDescriptor& optionDescriptor) {
    return optionDescriptor.type == CLIOptionType::Flag;
}
/**
 * @brief Checks if a given CLIOptionDescriptor corresponds to a counter option.
 * @param optionDescriptor The CLIOptionDescriptor to check.
 * @return True if the CLIOptionDescriptor corresponds to a counter option, false otherwise.
 */
inline bool isCounterOption(const CLIOptionDescriptor& optionDescriptor) {
    return optionDescriptor.type == CLIOptionType::Counter;
}
/**
 * @brief Checks if a given CLIOptionDescriptor corresponds to a value option.
 * @param optionDescriptor The CLIOptionDescriptor to check.
 * @return True if the CLIOptionDescriptor corresponds to a value option, false otherwise.
 */
inline bool isValueOption(const CLIOptionDescriptor& optionDescriptor) {
    return optionDescriptor.type == CLIOptionType::Value;
}
/**
 * @brief Checks if a given CLIOptionDescriptor corresponds to a list option.
 * @param optionDescriptor The CLIOptionDescriptor to check.
 * @return True if the CLIOptionDescriptor corresponds to a list option, false otherwise.
 */
inline bool isListOption(const CLIOptionDescriptor& optionDescriptor) {
    return optionDescriptor.type == CLIOptionType::List;
}

} // namespace descriptor
} // namespace cli
VEEC_NAMESPACE_END
