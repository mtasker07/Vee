/**
 * @file SourceManager.hpp
 * @brief This file contains the main interface for the vee compiler.
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <system_error>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Maybe.hpp"
#include "veec/basic/Result.hpp"
#include "veec/fs/Path.hpp"
#include "veec/source/SourceFileId.hpp"
#include "veec/source/SourceFile.hpp"
#include "veec/source/SourceLocation.hpp"
#include "veec/source/SourceRange.hpp"
#include "veec/source/SourceView.hpp"

VEEC_NAMESPACE_BEGIN
namespace source {

enum class SourceLoadErrorKind {
    FileNotFound,
    PermissionDenied,
    UnknownError
};

struct SourceLoadError {
	SourceLoadErrorKind kind = SourceLoadErrorKind::UnknownError;
    fs::Path filePath;
    std::error_code systemError;
};

/**
 * @class SourceManager
 * @brief The SourceManager class is responsible for managing source files and their contents during the compilation process.
 * It provides functionality to load source files, retrieve their contents, and manage their lifetimes.
 */
class SourceManager {
public:
    /**
     * @brief Constructs a new SourceManager instance.
     */
    SourceManager() = default;

    /**
     * @brief Loads a source file from the given path and returns its contents as a string_view.
     *
     * @param path The path to the source file to load.
     * @return The SourceFileId of the loaded source file.
     */
    basic::Result<SourceFileId, SourceLoadError> loadFile(const fs::Path& path);
    /**
     * @brief Adds a virtual source file with the given name and contents.
     * 
     * @param name The name of the virtual source file.
     * @param contents The contents of the virtual source file.
     * @return The SourceFileId of the added virtual source file.
     */
    SourceFileId addVirtualFile(fs::Path::StringViewType name, std::string_view contents);

    /**
     * @brief Creates a SourceLocation for the given SourceFileId and offset.
     * 
     * @param fileId The SourceFileId of the source file.
     * @param offset The offset within the source file.
     * @return The SourceLocation corresponding to the given SourceFileId and offset.
     */
    SourceLocation location(SourceFileId fileId, u32 offset) const;
    /**
     * @brief Creates a SourceRange for the given SourceFileId and start/end offsets.
     * 
     * @param fileId The SourceFileId of the source file.
     * @param startOffset The starting offset within the source file.
     * @param endOffset The ending offset within the source file.
     * @return The SourceRange corresponding to the given SourceFileId and offsets.
     */
    SourceRange range(SourceFileId fileId, u32 startOffset, u32 endOffset) const;

    /**
     * @brief Converts a SourceLocation to a LineColumn representation.
     * 
     * @param loc The SourceLocation to convert.
     * @return The LineColumn representation of the given SourceLocation.
     */
    LineColumn lineColumn(SourceLocation loc) const;

    /**
     * @brief Retrieves the contents of the source file corresponding to the given SourceFileId.
     * 
     * @param fileId The SourceFileId of the source file.
     * @return A string_view representing the contents of the source file.
     */
    std::string_view getContents(SourceFileId fileId) const;
    /**
     * @brief Retrieves a SourceView for the source file corresponding to the given SourceFileId.
	 * @param fileId The SourceFileId of the source file.
	 * @return A SourceView representing the source file.
     */
	SourceView getView(SourceFileId fileId) const;
    /**
     * @brief Retrieves a slice of the contents of the source file corresponding to the given SourceFileId and range.
     * 
     * @param fileId The SourceFileId of the source file.
     * @param range The range within the source file.
     * @return A string_view representing the slice of the source file.
     */
    std::string_view getText(SourceRange range) const;
    /**
     * @brief Retrieves the file path of the source file corresponding to the given SourceFileId.
     * @param fileId The SourceFileId of the source file.
     * @return The path of the source file, if it exists.
     */
    basic::Maybe<fs::Path> getPath(SourceFileId fileId) const;

    /**
     * @brief Retrieves the SourceFile corresponding to the given SourceFileId (immutable).
     * 
     * @param fileId The SourceFileId of the source file.
     * @return A const reference to the SourceFile.
     */
    const SourceFile& getFile(SourceFileId fileId) const;
    /**
      * @brief Retrieves the SourceFile corresponding to the given SourceFileId.
      *
      * @param fileId The SourceFileId of the source file.
      * @return A reference to the SourceFile.
      */
	SourceFile& getFile(SourceFileId fileId);

    /**
     * @brief Retrieves a list of all SourceFileIds managed by the SourceManager.
     * @return A list of all SourceFileIds.
     */
    std::vector<SourceFileId> getAllFileIds() const;

private:
    std::vector<SourceFile> _files;
    std::unordered_map<fs::Path, SourceFileId> _fileMap;
};

} // namespace source
VEEC_NAMESPACE_END
