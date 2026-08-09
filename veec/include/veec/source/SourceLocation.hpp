/**
 * @file SourceLocation.hpp
 * @brief This file contains the definition of the SourceLocation struct,
 * which represents a specific location in a source file.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace source {

class SourceFile;

/**
 * @brief Represents a specific location in a source file.
 */
struct SourceLocation {
    /**
     * @brief The source file this location belongs to.
     */
    SourceFile* file = nullptr;
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
