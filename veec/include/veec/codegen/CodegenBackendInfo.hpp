/**
 * @file CodegenBackendInfo.hpp
 * @brief This file contains the definition of the CodegenBackendInfo struct,
 * which stores information about a certain codegen backend.
 */

#pragma once

#include <string>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/CodegenResultType.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {

/**
 * @struct CodegenBackendInfo
 * @brief Stores info about a certain codegen backend.
 */
struct CodegenBackendInfo {
    /**
     * @brief Identifier for the backend, should be unique, all lowercase and contain
     * no spaces. E.g., "llvm", "wasm".
     */
    std::string_view identifier;
    /**
     * @brief Display name for the backend, e.g., "LLVM", "WebAssembly".
     */
    std::string_view name;
    /**
     * @brief Description of the backend.
     */
    std::string_view description;
};

} // namespace codegen
VEEC_NAMESPACE_END
