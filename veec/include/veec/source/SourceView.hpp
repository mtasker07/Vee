/**
 * @file SourceView.hpp
 * @brief Represents a view into a source file, containing a span of characters and the associated file ID.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/source/SourceFileId.hpp"
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
    SourceView(const SourceFile& file)
        : data(file.getContents().data()), length(file.getLength()), fileId(file.getId()) {}
    /**
     * @brief Constructs a SourceView from a string view and a file ID.
     * @param str The string view representing the source data.
     * @param id The ID of the source file.
     */
    SourceView(std::string_view str, SourceFileId id)
        : data(str.data()), length(static_cast<u32>(str.size())), fileId(id) {}

    /**
     * @brief Returns a pointer to the underlying character data.
     * It is generally recommended to use toStringView() rather than
     * the character array directly.
     * @return A pointer to the character data.
     */
    inline const char* getData() const { return data; }
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
    inline SourceFileId getFileId() const { return fileId; }

    /**
     * @brief Returns a string view representing the source data.
     * @return A string view of the source data.
     */
    inline std::string_view str() const {
        return std::string_view(data, length);
    }

private:
    const char* data = nullptr;
    u32 length = 0;
    SourceFileId fileId;
};

} // namespace source
VEEC_NAMESPACE_END
