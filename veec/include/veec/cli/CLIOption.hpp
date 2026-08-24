/**
 * @file CLIOption.hpp
 * @brief This file contains the definition of the CLIOption enum.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

/**
 * @enum CLIOption
 * @brief Represents the different command-line options that can be specified.
 * Unknown is a special option that is used when an unrecognized option is encountered by the parser.
 */
enum class CLIOption : u8 {
    Unknown, // Unknown option

    // Global
    Version,

    // Compile
    InputFile,
    OutputFile,
    OptimizationLevel,
    OutputMir,
};

/**
 * @brief Checks if a given CLIOption is a flag option.
 * @param option The CLIOption to check.
 * @return True if the CLIOption is a flag option, false otherwise.
 */
bool isFlagOption(const CLIOption option);
/**
 * @brief Checks if a given CLIOption is a counter option.
 * @param option The CLIOption to check.
 * @return True if the CLIOption is a counter option, false otherwise.
 */
bool isCounterOption(const CLIOption option);
/**
 * @brief Checks if a given CLIOption is a value option.
 * @param option The CLIOption to check.
 * @return True if the CLIOption is a value option, false otherwise.
 */
bool isValueOption(const CLIOption option);
/**
 * @brief Checks if a given CLIOption is a list option.
 * @param option The CLIOption to check.
 * @return True if the CLIOption is a list option, false otherwise.
 */
bool isListOption(const CLIOption option);

} // namespace cli
VEEC_NAMESPACE_END
