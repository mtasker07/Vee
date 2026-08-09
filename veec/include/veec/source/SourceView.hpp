/**
 * @file SourceView.hpp
 * @brief Represents a view into a source file, containing a span of characters and the associated file ID.
 */

#pragma once

#include <string_view>
#include <span>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/source/SourceFile.hpp"

VEEC_NAMESPACE_BEGIN
namespace source {

/**
 * @class SourceView
 * @brief Represents a view into a source file, containing a span of characters and
 * the associated file ID.
 */
class SourceView {
public:
    SourceView() = default;
    /**
     * @brief Constructs a SourceView from a SourceFile.
     * @param file The source file to create a view from.
     */
    SourceView(SourceFile* file)
        : file(file), data(std::span<const char>(file->getContents().data(), file->getLength())), length(file->getLength()) {}
    /**
     * @brief Constructs a SourceView from a slice of a SourceFile.
     * @param file The source file to create a view from.
     * @param begin The starting offset of the slice in the source file.
     * @param end The ending offset of the slice in the source file.
     */
    SourceView(SourceFile* file, u32 begin, u32 end)
        : file(file), data(std::span<const char>(file->getContents().data() + begin, end - begin)), length(end - begin) {
        VEE_ASSERT(file != nullptr, "SourceView file cannot be null");
        VEE_ASSERT(end >= begin, "SourceView end must be >= to begin");
        VEE_ASSERT(end <= file->getLength(), "SourceView end must be <= to file length");
    }

    /**
     * @brief Returns the underlying character span of this view.
     * @return A span representing the character data.
     */
    inline std::span<const char> getData() const { return data; }
    /**
     * @brief Returns the length of the source view.
     * @return The length of the source view.
     * @note This function returns a u32. If you need the length as a size_t, use getSize() instead.
     */
    inline u32 getLength() const { return length; }
    /**
     * @brief Returns the length of the source view as a size_t.
     * @return The length of the source view.
     * @note This function returns a size_t. If you need the length as a u32, use getLength() instead.
     */
    inline size_t getSize() const { return static_cast<size_t>(length); }
    /**
     * @brief Returns the ID of the source file associated with this view.
     * @return The source file ID.
     */
    inline SourceFile* getFile() const { return file; }

    /**
     * @brief Returns a string view representing the source data.
     * @return A string view of the source data.
     */
    inline std::string_view str() const {
        return std::string_view(data.data(), data.size());
    }

private:
    SourceFile* file = nullptr;
    std::span<const char> data = {};
    u32 length = 0;
};

} // namespace source
VEEC_NAMESPACE_END
