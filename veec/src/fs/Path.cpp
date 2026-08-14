#include "veec/fs/Path.hpp"

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace fs {

char Path::getNativePathSeparator() {
    constexpr wchar_t w = std::filesystem::path::preferred_separator;
    static_assert(w > 0 && w <= 127, "Invalid platform path separator character");

    return static_cast<char>(w);
}

bool Path::exists() const {
    return std::filesystem::exists(_path);
}

bool Path::isFile() const {
    return std::filesystem::is_regular_file(_path);
}
bool Path::isDirectory() const {
    return std::filesystem::is_directory(_path);
}

//
// Path slicing
//

fs::Path Path::getParentDirectory() const {
    return Path(_path.parent_path());
}
fs::Path Path::getFileName() const {
    return Path(_path.filename());
}

void Path::resolveSymbolicLinks() {
    VEE_ASSERT(exists(), "Path does not exist, cannot resolve symbolic links");

    _path = std::filesystem::canonical(_path);
}

void Path::normalize() {
    if (_path.empty()) return;

    _path = std::filesystem::absolute(_path).lexically_normal();
    _path.make_preferred();
}

} // namespace fs
VEEC_NAMESPACE_END
