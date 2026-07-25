/**
 * @file SourceLocation.hpp
 * @brief This file contains the definition of the SourceLocation struct,
 * which represents a specific location in a source file.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/source/SourceFileId.hpp"

VEEC_NAMESPACE_BEGIN
namespace source {

/**
 * @brief Represents a specific location in a source file.
 */
struct SourceLocation {
    /**
     * @brief The ID of the source file this location belongs to.
     */
    SourceFileId fileId;
    /**
     * @brief The offset of the location in the source file.
     */
    u32 offset;
};

/**
 * @brief Represents a line and column in a source file.
 */
struct LineColumn {
    /**
     * @brief The line number (1-based).
     */
    u32 line;
    /**
     * @brief The column number (1-based).
     */
    u32 column;
};

} // namespace source
VEEC_NAMESPACE_END
