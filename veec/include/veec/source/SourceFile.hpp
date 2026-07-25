/**
 * @file SourceFile.hpp
 * @brief This file contains the declaration of the SourceFile class.
 */

#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <algorithm>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/fs/Path.hpp"
#include "veec/source/SourceFileId.hpp"
#include "veec/source/SourceLocation.hpp"

VEEC_NAMESPACE_BEGIN
namespace source {

class SourceFile {
public:
    SourceFile(const SourceFile& other) = delete;
    SourceFile& operator=(const SourceFile& other) = delete;
    
    SourceFile(SourceFile&& other) noexcept
        : _fileId(other._fileId),
          _filePath(std::move(other._filePath)),
          _contents(std::move(other._contents)) {
        other._fileId = 0;
    }
    SourceFile& operator=(SourceFile&& other) noexcept {
        if (this != &other) {
            _fileId = other._fileId;
            _filePath = std::move(other._filePath);
            _contents = std::move(other._contents);
            other._fileId = 0;
        }
        return *this;
    }

    ~SourceFile() = default;

    /**
     * @brief Returns the unique identifier of the source file.
     * @return The SourceFileId of the source file.
     */
    inline SourceFileId getId() const {
        return _fileId;
    }

    /**
     * @brief Returns the path of the source file.
     * @return The path of the source file.
     */
    inline const fs::Path& getPath() const {
        return _filePath;
    }
    /**
     * @brief Returns the contents of the source file.
     * @return The contents of the source file as a string_view.
     */
    inline std::string_view getContents() const {
        return _contents;
    }
    /**
     * @brief Returns the length of the source file contents.
     * @return The length of the source file contents.
     */
    inline u32 getLength() const {
        return static_cast<u32>(_contents.size());
    }

    /**
     * @brief Returns the line and column corresponding to the given offset in the source file.
     * @param offset The offset in the source file.
     * @return The line and column corresponding to the offset.
     */
    inline LineColumn getLineColumn(u32 offset) const {
        if (_lineOffsets.empty()) {
            buildLineTable();
        }

        auto it = std::upper_bound(
            _lineOffsets.begin(),
            _lineOffsets.end(),
            offset);

        uint32_t line = static_cast<uint32_t>(it - _lineOffsets.begin()) - 1;
        uint32_t column = offset - _lineOffsets[line];

        return {
            .line = line + 1,
            .column = column + 1,
        };
    }

private:
    friend class SourceManager;

    SourceFileId _fileId;
    fs::Path _filePath;
    std::string _contents;

	// Only constructable by SourceManager
    SourceFile(SourceFileId fileId, const fs::Path& filePath, std::string contents) {
        _fileId = fileId;
        _filePath = filePath;
        _contents = std::move(contents);
    }

    // Cached line offsets
    mutable std::vector<u32> _lineOffsets;

    void buildLineTable() const;
};

} // namespace source
VEEC_NAMESPACE_END
