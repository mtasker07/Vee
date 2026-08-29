/**
 * @file ObjectFormat.hpp
 * @brief This file contains the definition of the ObjectFormat enum which contains
 * all supported target object file formats.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace target {

/**
 * @enum ObjectFormat
 * @brief Represents the target object file format for code generation.
 */
enum class ObjectFormat : u8 {
    /**
     * @brief Unknown object file format (sentinel).
     */
    Unknown,
    /**
     * @brief Executable and Linkable Format (ELF), used by Linux.
     */
    ELF,
    /**
     * @brief Common Object File Format (COFF), used by Windows.
     */
    COFF,
    /**
     * @brief Mach-O format, used by macOS.
     */
    MachO
};

} // namespace target
VEEC_NAMESPACE_END
