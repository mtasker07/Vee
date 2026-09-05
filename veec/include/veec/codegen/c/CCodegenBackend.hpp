/**
 * @file CCodegenBackend.hpp
 * @brief This file contains the definition of the CCodegenBackend class which is
 * a subclass of CodegenBackend for generating C code.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/codegen/CodegenBackend.hpp"
#include "veec/codegen/CodegenResult.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {

/**
 * @class CCodegenBackend
 * @brief A code generator backend for producing C code.
 */
class CCodegenBackend final : public CodegenBackend {
public:
    /**
     * @brief Creates a new CCodegenBackend instance with the given context.
     * @param ctx The compilation context object.
     */
    CCodegenBackend(compilation::CompilationContext& ctx)
        : CodegenBackend(ctx) {}

    virtual ~CCodegenBackend() = default;

    /**
     * @brief Queries information about the C code generation backend.
     * @return A CodegenBackendInfo containing information about the backend.
     */
    const CodegenBackendInfo& queryBackendInfo() const override;

    /**
     * @brief Generates C code for the given modules.
     * @param modules The modules to generate code for.
     * @return The result of the code generation process.
     */
    CodegenResult generate(const std::vector<const mir::Module*>& modules) override;
};

} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
