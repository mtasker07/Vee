/**
 * @file Keywords.hpp
 * @brief This file contains all the keyword constants used
 * in the Vee programming language.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace constants {

namespace keywords {
    inline constexpr std::string_view FUNC = "func";
    inline constexpr std::string_view IF = "if";
    inline constexpr std::string_view ELSE = "else";
    inline constexpr std::string_view LOOP = "loop";
    inline constexpr std::string_view WHILE = "while";
    inline constexpr std::string_view FOR = "for";
    inline constexpr std::string_view RETURN = "return";
}

} // namespace constants
VEEC_NAMESPACE_END
