/**
 * @file Arch.hpp
 * @brief This file contains the definition of the Arch enum which contains
 * all supported target architectures.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace target {

/**
 * @enum Arch
 * @brief Represents the target architecture for code generation.
 */
enum class Arch : u8 {
    /**
     * @brief Unknown architecture (sentinel).
     */
    Unknown,
    /**
     * @brief 32-bit Intel 386 architecture, more commonly known as x86 or i386.
     */
    I386,
    /**
     * @brief 64-bit AMD64 architecture, more commonly known as x86_64 or x64.
     */
    AMD64
};

} // namespace target
VEEC_NAMESPACE_END
