#include "veec/basic/StringPool.hpp"

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"

VEEC_NAMESPACE_BEGIN
namespace basic {

StringId StringPool::intern(std::string_view str) {
    auto it = _stringToId.find(std::string(str));
    if (it != _stringToId.end()) {
        return it->second;
    }

    StringId id = static_cast<StringId>(_strings.size());
    _strings.emplace_back(str);
    _stringToId.emplace(_strings.back(), id);
    return id;
}
std::string_view StringPool::get(StringId id) const {
    VEE_ASSERT(id < _strings.size(), "Invalid StringId");
    return _strings[id];
}

} // namespace basic
VEEC_NAMESPACE_END
