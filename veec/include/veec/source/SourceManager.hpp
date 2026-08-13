/**
 * @file SourceManager.hpp
 * @brief This file contains the SourceManager class which manages source files
 * during compilation.
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <system_error>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Result.hpp"
#include "veec/basic/Arena.hpp"
#include "veec/fs/Path.hpp"
#include "veec/source/SourceFile.hpp"
#include "veec/source/SourceLocation.hpp"
#include "veec/source/SourceRange.hpp"
#include "veec/source/SourceView.hpp"

VEEC_NAMESPACE_BEGIN
namespace source {

/**
 * @enum SourceLoadErrorKind
 * @brief Represents an error that occurred while loading a source file.
 */
enum class SourceLoadErrorKind {
    FileNotFound,
    PermissionDenied,
    UnknownError
};

/**
 * @struct SourceLoadError
 * @brief Stores data about an error that occurred while loading a source file.
 */
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
     * @param path The path to the source file to load.
     * @return A pointer to the new loaded source file instance, or a SourceLoadError on error.
     */
    basic::Result<SourceFile*, SourceLoadError> loadFile(const fs::Path& path);
    /**
     * @brief Adds a virtual source file with the given name and contents.
     * @param name The name of the virtual source file.
     * @param contents The contents of the virtual source file.
     * @return A pointer to the new virtual source file instance.
     */
    SourceFile* addVirtualFile(const fs::Path& name, std::string_view contents);

    /**
     * @brief Creates a SourceLocation for the given SourceFileId and offset.
     * @param file The source file.
     * @param offset The offset within the source file.
     * @return The SourceLocation corresponding to the given source file and offset.
     */
    SourceLocation location(SourceFile* file, u32 offset) const;
    /**
     * @brief Creates a SourceRange for the given source file and start/end offsets.
     * @param file The source file.
     * @param startOffset The starting offset within the source file.
     * @param endOffset The ending offset within the source file.
     * @return The SourceRange corresponding to the given source file and offsets.
     */
    SourceRange range(SourceFile* file, u32 startOffset, u32 endOffset) const;

    /**
     * @brief Converts a SourceLocation to a LineColumn representation.
     * @param loc The SourceLocation to convert.
     * @return The LineColumn representation of the given SourceLocation.
     */
    LineColumn lineColumn(SourceLocation loc) const;

    /**
     * @brief Retrieves a list of all SourceFiles managed by this SourceManager.
     * @return A list of all SourceFiles.
     */
    const std::vector<SourceFile*>& getAllFiles() const {
        return _files;
    }

private:
    basic::Arena<> _fileArena;
    std::vector<SourceFile*> _files;
    std::unordered_map<fs::Path, SourceFile*> _fileMap;
};

} // namespace source
VEEC_NAMESPACE_END
