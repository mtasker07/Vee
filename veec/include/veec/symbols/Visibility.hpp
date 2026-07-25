/**
 * @file Visibility.hpp
 * @brief This file contains the definition of the Visibility enum.
 *
 * The Visibility enum represents the visibility of a symbol in the program.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

/**
 * @enum Visibility
 * @brief Represents the visibility of a symbol.
 */
enum class Visibility {
    Public,
    Private,
};

} // namespace symbols
VEEC_NAMESPACE_END
