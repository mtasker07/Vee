#include "veec/codegen/c/CCodegenBackend.hpp"

#include <string>
#include <vector>
#include <format>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/Module.hpp"
#include "veec/codegen/CodegenBackend.hpp"
#include "veec/codegen/CodegenBackendInfo.hpp"
#include "veec/codegen/CodegenResult.hpp"
#include "veec/codegen/CodegenResultType.hpp"
#include "veec/codegen/c/CCodegenContext.hpp"
#include "veec/codegen/c/CConstructGenerator.hpp"
#include "veec/codegen/c/CConstructPrinter.hpp"
#include "veec/codegen/c/construct/CConstructFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {

const CodegenBackendInfo& CCodegenBackend::queryBackendInfo() const {
    static CodegenBackendInfo info = {
        /* identifier           */ "c",
        /* name                 */ "C",
        /* description          */ "C code generation backend"
    };
    return info;
}

CodegenResult CCodegenBackend::generate(const std::vector<const mir::Module*>& modules) {
    CodegenResult result(CodegenResultType::LanguageCode);
    
    CCodegenContext cctx;

    // Generate constructs
    CConstructGenerator constructGen(_ctx, cctx);
    const construct::CCompilationUnit unit = constructGen.generateModules(modules);

    // Print constructs
    CConstructPrinter printer(_ctx, cctx);
    result.setData(printer.printCompilationUnit(unit));
    
    result.setSucceeded(true);
    return result;
}

} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
