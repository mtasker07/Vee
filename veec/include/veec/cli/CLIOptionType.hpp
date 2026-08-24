/**
 * @file CLIOptionType.hpp
 * @brief This file contains the definition of the CLIOptionType enum.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

/**
 * @enum CLIOptionType
 * @brief Represents the type of a command-line option.
 */
enum class CLIOptionType : u8 {
    /**
     * @brief Flags are options that do not take any values and can only be specified once.
     * They are typically used to enable or disable a feature, and their behaviour is always
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
    /**
     * @brief List options can be specified multiple times and collect required values into a list.
     */
    List,
};

} // namespace cli
VEEC_NAMESPACE_END
