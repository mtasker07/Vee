/**
 * @file FlowInfo.hpp
 * @brief This file contains the definition of the FlowInfo struct.
 * The FlowInfo struct represents information about the control flow
 * for a specific node.
 * 
 * The default values for FlowInfo should ALWAYS be a fallthrougable node with no
 * exit statements. Since we use default construction a lot and dont set them explicitly.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema {

/**
 * @struct FlowInfo
 * @brief Represents information about the control flow for a specific node.
 */
struct FlowInfo {
    bool canFallThrough = true;
    bool canReturn = false;
    bool canBreak = false;
    bool canContinue = false;
};

} // namespace sema
VEEC_NAMESPACE_END
