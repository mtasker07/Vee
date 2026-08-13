/**
 * @file SourceManager.cpp
 * @brief This file contains the implementation of the SourceManager class. 
 */

#include "veec/source/SourceManager.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Arena.hpp"
#include "veec/basic/Result.hpp"
#include "veec/fs/Path.hpp"
#include "veec/source/SourceFile.hpp"
#include "veec/source/SourceLocation.hpp"
#include "veec/source/SourceRange.hpp"
#include "veec/source/SourceView.hpp"

VEEC_NAMESPACE_BEGIN
namespace source {

basic::Result<SourceFile*, SourceLoadError> SourceManager::loadFile(const fs::Path& path) {
    // File already loaded?
    auto it = _fileMap.find(path);
    if (it != _fileMap.end()) {
        return it->second;
    }

    // Load the file contents
    // TODO: abstract into file system class/subsystem
    std::string contents;
    try {
        std::ifstream file(path.str().data());
        if (!file) {
            SourceLoadError error;
            error.kind = SourceLoadErrorKind::FileNotFound;
            error.filePath = path;
            return error;
        }
        contents.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    } catch (const std::exception&) {
        SourceLoadError error;
        error.kind = SourceLoadErrorKind::UnknownError;
        error.filePath = path;
        return error;
    }

    // Create new file
    SourceFile* source = _fileArena.create<SourceFile>(path, std::move(contents));
    _files.push_back(source);
    _fileMap[path] = source;

    return source;
}
SourceFile* SourceManager::addVirtualFile(const fs::Path& name, std::string_view contents) {
    // File already loaded?
    auto it = _fileMap.find(name);
    if (it != _fileMap.end()) {
        return it->second;
    }

    // Create new file
    SourceFile* source = _fileArena.create<SourceFile>(name, std::string(contents));
    _files.push_back(source);
    _fileMap[name] = source;

    return source;
}

SourceLocation SourceManager::location(SourceFile* file, u32 offset) const {
    return SourceLocation{file, offset};
}
SourceRange SourceManager::range(SourceFile* file, u32 startOffset, u32 endOffset) const {
    return SourceRange(file, startOffset, endOffset);
}

LineColumn SourceManager::lineColumn(SourceLocation loc) const {
    return loc.file->getLineColumn(loc.offset);
}

} // namespace source
VEEC_NAMESPACE_END
