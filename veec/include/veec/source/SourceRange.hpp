/**
 * @file SourceRange.hpp
 * @brief This file contains the definition of the SourceRange struct,
 * which represents a range of source code in a source file.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/source/SourceFileId.hpp"

#if VEEC_DEBUG
#include <string_view>
#endif

VEEC_NAMESPACE_BEGIN
namespace source {

/**
 * @brief Represents a range of source code in a source file.
 */
struct SourceRange {
    /**
     * @brief The ID of the source file this range belongs to.
     */
    SourceFileId fileId = 0;
    /**
     * @brief The starting offset of the range in the source file.
     */
    u32 startOffset = 0;
    /**
     * @brief The ending offset of the range in the source file.
     */
    u32 endOffset = 0;

#if VEEC_DEBUG
    /**
     * @brief (DEBUG ONLY) A view of the source text for this range,
     * used for debugging purposes only.
     */
    std::string_view dbgTextView;
#endif
};

} // namespace source
VEEC_NAMESPACE_END
