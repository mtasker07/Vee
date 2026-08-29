/**
 * @file Target.hpp
 * @brief This file contains the definition of the Target struct which contains
 * information for a specific target.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/target/Arch.hpp"
#include "veec/target/OS.hpp"
#include "veec/target/ABI.hpp"
#include "veec/target/ObjectFormat.hpp"

VEEC_NAMESPACE_BEGIN
namespace target {

/**
 * @struct Target
 * @brief Represents a specific target for code generation, including architecture, operating system,
 * ABI, and object file format.
 */
struct Target {
    /**
     * @brief The target architecture.
     */
    Arch arch = Arch::Unknown;
    /**
     * @brief The target operating system.
     */
    OS os = OS::Unknown;
    /**
     * @brief The target ABI.
     */
    ABI abi = ABI::Unknown;
    /**
     * @brief The target object file format.
     */
    ObjectFormat objectFormat = ObjectFormat::Unknown;
};

} // namespace target
VEEC_NAMESPACE_END
