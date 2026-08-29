/**
 * @file ABI.hpp
 * @brief This file contains the definition of the ABI enum which contains
 * all supported target ABIs.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace target {

/**
 * @enum ABI
 * @brief Represents the target ABI for code generation.
 */
enum class ABI : u8 {
    /**
     * @brief Unknown ABI (sentinel).
     */
    Unknown,
    /**
     * @brief Microsoft x64 calling convention.
     */
    MicrosoftX64,
    /**
     * @brief System V AMD64 calling convention.
     */
    SystemVAMD64
};

} // namespace target
VEEC_NAMESPACE_END
