#include "veec/io/FileWriter.hpp"

#include <fstream>
#include <string>
#include <string_view>
#include <filesystem>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace io {

void FileWriter::write(std::string_view str) {
    VEE_ASSERT(_fileStream.is_open(), "File stream is not open");
    
    _fileStream << str;
}
void FileWriter::flush() {
    VEE_ASSERT(_fileStream.is_open(), "File stream is not open");

    _fileStream.flush();
}

void FileWriter::open(
    const fs::Path& filePath,
    std::ios_base::openmode mode,
    bool createDirectories
) {
    VEE_ASSERT(!_fileStream.is_open(), "File stream is already open");

    // TODO: This should all be in the Path API so we dont need to use filesystem
    if (createDirectories) {
		fs::Path parentDir = filePath.getParentDirectory();
        if (!parentDir.isEmpty() && !parentDir.exists()) {
            std::filesystem::create_directories(parentDir.filesystemPath());
        }
    }

    _fileStream.open(filePath.toString(), mode);
    VEE_ASSERT(_fileStream.is_open(), "Failed to open file: {}", filePath.toString());
}
void FileWriter::close() {
    if (_fileStream.is_open()) {
        _fileStream.close();
    }
}

} // namespace io
VEEC_NAMESPACE_END
