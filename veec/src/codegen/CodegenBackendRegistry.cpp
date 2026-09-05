#include "veec/codegen/CodegenBackendRegistry.hpp"

#include <string>
#include <string_view>
#include <unordered_map>
#include <type_traits>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/CodegenBackend.hpp"
#include "veec/codegen/CodegenBackendInfo.hpp"

// Backends
#include "veec/codegen/c/CCodegenBackend.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {

template<typename T>
void CodegenBackendRegistry::registerBackend() {
    static_assert(std::is_base_of_v<CodegenBackend, T>, "T must be a subclass of CodegenBackend");

    auto backend = std::make_unique<T>(_ctx);
    const std::string_view identifier = backend->queryBackendInfo().identifier;
    _backends.emplace(identifier, std::move(backend));
}

void CodegenBackendRegistry::registerAllDefaults() {
    registerBackend<c::CCodegenBackend>();
}

CodegenBackend* CodegenBackendRegistry::getDefaultBackend() const {
    if (_backends.empty()) {
        return nullptr;
    }
    return _backends.begin()->second.get();
}
CodegenBackend* CodegenBackendRegistry::getBackend(std::string_view identifier) const {
    auto it = _backends.find(identifier);
    if (it != _backends.end()) {
        return it->second.get();
    }
    return nullptr;
}

} // namespace codegen
VEEC_NAMESPACE_END
