/**
 * @file SourceManager.cpp
 * @brief This file contains the implementation of the SourceManager class. 
 */

#include "veec/source/SourceManager.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Result.hpp"
#include "veec/fs/Path.hpp"
#include "veec/source/SourceFileId.hpp"
#include "veec/source/SourceFile.hpp"
#include "veec/source/SourceLocation.hpp"
#include "veec/source/SourceRange.hpp"
#include "veec/source/SourceView.hpp"

VEEC_NAMESPACE_BEGIN
namespace source {

basic::Result<SourceFileId, SourceLoadError> SourceManager::loadFile(const fs::Path& path) {
    // Check if the file is already loaded
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

    // Create a new SourceFile and store it
    SourceFileId fileId = static_cast<SourceFileId>(_files.size());
    SourceFile source = SourceFile(fileId, path, contents);
    _files.push_back(std::move(source));
    _fileMap[path] = fileId;

    return fileId;
}
SourceFileId SourceManager::addVirtualFile(fs::Path::StringViewType name, std::string_view contents) {
    fs::Path path(name);
    // Check if the file is already loaded
    auto it = _fileMap.find(path);
    if (it != _fileMap.end()) {
        return it->second;
    }

    // Create a new SourceFile and store it
	SourceFileId fileId = static_cast<SourceFileId>(_files.size());
	SourceFile source = SourceFile(fileId, path, std::string(contents));
    _files.push_back(std::move(source));
    _fileMap[path] = fileId;

    return fileId;
}

SourceLocation SourceManager::location(SourceFileId fileId, u32 offset) const {
    return SourceLocation{fileId, offset};
}
SourceRange SourceManager::range(SourceFileId fileId, u32 startOffset, u32 endOffset) const {
#if VEEC_DEBUG
    const SourceFile& file = getFile(fileId);
    std::string_view dbgText = file.getContents().substr(startOffset, endOffset - startOffset);
    return SourceRange{ fileId, startOffset, endOffset, dbgText };
#else
    return SourceRange{fileId, startOffset, endOffset};
#endif
}

LineColumn SourceManager::lineColumn(SourceLocation loc) const {
    const SourceFile& file = getFile(loc.fileId);
    return file.getLineColumn(loc.offset);
}

std::string_view SourceManager::getContents(SourceFileId fileId) const {
    const SourceFile& file = getFile(fileId);
    return file.getContents();
}
SourceView SourceManager::getView(SourceFileId fileId) const {
	const SourceFile& file = getFile(fileId);
	return SourceView(file);
}
std::string_view SourceManager::getText(SourceRange range) const {
    const SourceFile& file = getFile(range.fileId);
    return file.getContents().substr(range.startOffset, range.endOffset - range.startOffset);
}

const SourceFile& SourceManager::getFile(SourceFileId fileId) const {
    VEE_ASSERT(fileId < _files.size(), "Invalid SourceFileId: {}", (u32)fileId);
    return _files.at(fileId);
}
SourceFile& SourceManager::getFile(SourceFileId fileId) {
    VEE_ASSERT(fileId < _files.size(), "Invalid SourceFileId: {}", (u32)fileId);
    return _files.at(fileId);
}

} // namespace source
VEEC_NAMESPACE_END
