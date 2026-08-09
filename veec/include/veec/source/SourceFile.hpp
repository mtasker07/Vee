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
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/fs/Path.hpp"
#include "veec/source/SourceLocation.hpp" // < for LineColumn
#include "veec/diagnostics/DiagnosticSource.hpp"
#include "veec/diagnostics/DiagnosticRange.hpp"

VEEC_NAMESPACE_BEGIN

namespace basic {
    template<size_t BlockSize>
    class Arena;
}

namespace source {

class SourceRange;

class SourceFile : public diagnostics::DiagnosticSource {
public:
    SourceFile(const SourceFile& other) = delete;
    SourceFile& operator=(const SourceFile& other) = delete;
    
    SourceFile(SourceFile&& other) noexcept
        : _filePath(std::move(other._filePath)),
          _contents(std::move(other._contents)) {
    }
    SourceFile& operator=(SourceFile&& other) noexcept {
        if (this != &other) {
            _filePath = std::move(other._filePath);
            _contents = std::move(other._contents);
        }
        return *this;
    }

    ~SourceFile() = default;

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
     * @brief Returns the text corresponding to the given SourceRange.
     * @param range The SourceRange for which to retrieve the text.
     * @return A string_view of the text corresponding to the SourceRange.
     */
    inline std::string_view getText(const SourceRange& range) const {
        VEE_ASSERT(range.getFile() == this, "SourceRange file does not match this SourceFile");
        VEE_ASSERT(range.getBegin() <= range.getEnd(), "Invalid SourceRange: begin > end");
        VEE_ASSERT(range.getEnd() <= _contents.size(), "Invalid SourceRange: end > contents size");
		return getView(range.getBegin(), range.getEnd());
    }

    /**
     * @brief Returns the text of a specific line in the source file.
     * @param line The line number (1-based) to retrieve.
     * @param includeNewline Whether to include the newline character at the end of the line.
     * @return A string_view of the text of the specified line.
     */
    inline std::string_view getLineText(u32 line, bool includeNewline = false) const {
        VEE_ASSERT(line > 0 && line <= _lineOffsets.size(), "Invalid line number");

        if (_lineOffsets.empty()) {
            buildLineTable();
        }

        u32 begin = _lineOffsets[line - 1];
        u32 end = (line < _lineOffsets.size()) ? _lineOffsets[line] : static_cast<u32>(_contents.size());
        if (!includeNewline && end > begin && _contents[end - 1] == '\n') {
            --end;
        }
        return getView(begin, end);
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
    /**
     * @brief Returns the number of lines in the source file.
     * @return The number of lines in the source file.
     */
    inline size_t getLineCount() const {
        if (_lineOffsets.empty()) {
            buildLineTable();
        }
        return _lineOffsets.size();
    }

    //
    // DiagnosticSource
    //

    /**
     * @brief Gets the kind of this diagnostic source, which is SourceCode.
     */
    diagnostics::DiagnosticSourceKind getDiagnosticSourceKind() const override {
        return diagnostics::DiagnosticSourceKind::SourceCode;
    }
    /**
     * @brief Gets the text corresponding to a given DiagnosticRange.
     * @param range The DiagnosticRange for which to retrieve the text.
     * @return A string_view of the text corresponding to the DiagnosticRange.
     */
    std::string_view getDiagnosticRangeText(diagnostics::DiagnosticRange range) const override {
        VEE_ASSERT(range.getEnd() <= _contents.size(), "Invalid DiagnosticRange: end > contents size");
        return getView(range.getBegin(), range.getEnd());
    }

private:
    template<size_t>
    friend class basic::Arena; // For alloc
    friend class SourceManager;

    fs::Path _filePath;
    std::string _contents;
    
    // Cached line offsets
    mutable std::vector<u32> _lineOffsets;

	// Only constructable by SourceManager
    SourceFile(const fs::Path& filePath, std::string&& contents)
        : _filePath(filePath), _contents(std::move(contents)) {
    }

    void buildLineTable() const;

    std::string_view getView(u32 begin, u32 end) const {
		return std::string_view(_contents.data() + begin, end - begin);
    }
};

} // namespace source
VEEC_NAMESPACE_END
