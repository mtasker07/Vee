/**
 * @file CLIArgsToken.hpp
 * @brief This file contains the definition of the CLIArgsToken struct.
 * 
 * The CLIArgsToken struct is used for splitting up cli args into smaller chunks of data the parser
 * can more easily understand.
 */

#pragma once

#include <string>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

enum class CLIArgsTokenType : u8 {
    Unknown,

    Positional, // any non-option argument

    LongOption, // --option
    ShortOption, // -o
    ShortSequence, // -abc, -O2

    EndOfOptions, // --

    EndOfArgs
};

struct CLIArgsToken {
    CLIArgsTokenType type;
    std::string_view lexeme;
    size_t argvIndex;
    std::string_view optionName;
};

} // namespace cli
VEEC_NAMESPACE_END
