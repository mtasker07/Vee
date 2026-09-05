/**
 * @file CodegenBackend.hpp
 * @brief This file contains the definition of the CodegenBackend class which is
 * the base class for all code generator backends.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/codegen/CodegenBackendInfo.hpp"
#include "veec/codegen/CodegenResult.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {

/**
 * @class CodegenBackend
 * @brief Base class for all code generator backends.
 */
class CodegenBackend {
public:
    virtual ~CodegenBackend() = default;

    /**
     * @brief Queries information about the codegen backend.
     * @return A CodegenBackendInfo reference containing information about the backend.
     */
    virtual const CodegenBackendInfo& queryBackendInfo() const = 0;

    /**
     * @brief Generates code for the given MIR modules. The type of code generated
     * depends on the specific codegen backend.
     * @param modules The modules to generate code for.
     * @return A CodegenResult containing the generated code and any relevant information.
     */
    virtual CodegenResult generate(const std::vector<const mir::Module*>& modules) = 0;

protected:
    compilation::CompilationContext& _ctx;

    CodegenBackend(compilation::CompilationContext& ctx)
        : _ctx(ctx) {}
};

} // namespace codegen
VEEC_NAMESPACE_END
