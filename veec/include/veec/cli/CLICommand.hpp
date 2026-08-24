/**
 * @file CLICommand.hpp
 * @brief This file contains the definition of the CLICommand enum.
 */

#pragma once

#include <string_view>
#include <variant>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

/**
 * @enum CLICommand
 * @brief Represents the different commands that can be specified.
 * Unknown is a special command that is used when an unrecognized command is encountered by the parser.
 */
enum class CLICommand : u8 {
    Unknown, // Unknown command

    Compile
};

} // namespace cli
VEEC_NAMESPACE_END
