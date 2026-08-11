#include "veec/compilation/CompilerDriver.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <utility>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/Compilation.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/compilation/CompilationConfig.hpp"
#include "veec/io/StreamWriter.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/cli/args/CLIArgs.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/rendering/TerminalDiagnosticRenderer.hpp"

VEEC_NAMESPACE_BEGIN
namespace compilation {

int CompilerDriver::run(int argc, const char** argv) {    
    compilation::Compilation compilation;

    cli::args::CLIArgs cliArgs(argc, argv);
    cli::args::CLIOptions cliOptions = cliArgs.generateOptions(compilation.getContext());

    compilation::CompilationConfig config
        = compilation::CompilationConfig::fromCLIOptions(cliOptions);
    compilation.setConfig(config);

    compilation::CompilationResult result = compilation.compile();

    if (!result.success) {
        io::StreamWriter terminalWriter(std::cout);
        diagnostics::TerminalDiagnosticRenderer diagRenderer;
        for (const auto& diag : result.diagnostics) {
            diagRenderer.render(diag, terminalWriter);
            terminalWriter.write("\n");
        }
        return 1;
    }
    std::cout << "Compilation succeeded!" << std::endl;
    return 0;
}

} // namespace compilation
VEEC_NAMESPACE_END
