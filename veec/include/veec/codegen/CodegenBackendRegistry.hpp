/**
 * @file CodegenBackendRegistry.hpp
 * @brief This file contains the definition of the CodegenBackendRegistry class which manages
 * the registration and retrieval of code generation backends.
 */

#pragma once

#include <string_view>
#include <unordered_map>
#include <memory>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/codegen/CodegenBackend.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {

/**
 * @class CodegenBackendRegistry
 * @brief Manages the registration and retrieval of code generation backends.
 */
class CodegenBackendRegistry {
public:
    /**
     * @brief Creates a new CodegenBackendRegistry with the given context.
     * @param ctx The compilation context object.
     */
    CodegenBackendRegistry(compilation::CompilationContext& ctx)
        : _ctx(ctx) {}

    ~CodegenBackendRegistry() = default;

    /**
     * @brief Registers a new code generation backend of the specified type.
     * @tparam T The type of the code generation backend to register.
     */
    template<typename T>
    void registerBackend();

    /**
     * @brief Registers all default code generation backends.
     * 
     * Backends:
     * - CCodegenBackend
     */
    void registerAllDefaults();

    /**
     * @brief Retrieves the default code generation backend (the first one that
     * was registered).
     * @return A pointer to the default backend, may be null if no backends are
     * registered.
     */
    CodegenBackend* getDefaultBackend() const;
    /**
     * @brief Retrieves a registered code generation backend by its identifier.
     * @param identifier The identifier of the backend to retrieve.
     * @return A pointer to the requested backend, or nullptr if not found.
     */
    CodegenBackend* getBackend(std::string_view identifier) const;

private:
    compilation::CompilationContext& _ctx;

    std::unordered_map<std::string_view, std::unique_ptr<CodegenBackend>> _backends;
};

} // namespace codegen
VEEC_NAMESPACE_END
