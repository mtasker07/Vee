/**
 * @file SourceRange.hpp
 * @brief This file contains the definition of the SourceRange class,
 * which represents a range of source code in a specific source file.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace source {

class SourceFile;

/**
 * @brief Represents a range of source code in a specific source file.
 */
class SourceRange {
public:
    SourceRange() = default;
    
    /**
     * @brief Constructs a SourceRange with the given source file and offsets.
     * @param file The source file this range belongs to.
     * @param begin The starting offset of the range in the source file.
     * @param end The ending offset of the range in the source file.
     */
    SourceRange(SourceFile* file, u32 begin, u32 end)
        : _file(file), _begin(begin), _end(end) {
        VEE_ASSERT(_file != nullptr, "SourceRange file cannot be null");
        VEE_ASSERT(_end >= _begin, "SourceRange end must be >= begin");
    }

    /**
     * @brief Gets the source file associated with this range.
     * @return The source file associated with this range.
     */
    SourceFile* getFile() const { return _file; }
    /**
     * @brief Gets the starting offset of this range in the source file.
     * @return The starting offset of this range in the source file.
     */
    u32 getBegin() const { return _begin; }
    /**
     * @brief Gets the ending offset of this range in the source file.
     * @return The ending offset of this range in the source file.
     */
    u32 getEnd() const { return _end; }
    /**
     * @brief Retrieves the text corresponding to this SourceRange.
     * @return A string_view of the text corresponding to this SourceRange.
     */
    std::string_view getText() const;

private:
    SourceFile* _file = nullptr;
    u32 _begin = 0;
    u32 _end = 0;
};

} // namespace source
VEEC_NAMESPACE_END
