/**
 * @file OS.hpp
 * @brief This file contains the definition of the OS enum which contains
 * all supported target operating systems.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace target {

/**
 * @enum OS
 * @brief Represents the target operating system for code generation.
 */
enum class OS : u8 {
    /**
     * @brief Unknown operating system (sentinel).
     */
    Unknown,
    /**
     * @brief Microsoft Windows.
     */
    Windows,
    /**
     * @brief Linux.
     */
    Linux,
    /**
     * @brief Apple macOS.
     */
    MacOS
};

} // namespace target
VEEC_NAMESPACE_END
