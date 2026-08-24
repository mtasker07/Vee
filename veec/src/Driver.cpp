#include "veec/Driver.hpp"

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/io/StdStreams.hpp"
#include "veec/cli/CLI.hpp"
#include "veec/diagnostics/UserDiagnostic.hpp"
#include "veec/diagnostics/DiagnosticPrinter.hpp"

VEEC_NAMESPACE_BEGIN

int Driver::main(int argc, const char** argv) {
    cli::CLI cli;
    cli::CLIInvocation invocation = cli.parse(argc, argv);

	const std::vector<diagnostics::UserDiagnostic>& diagnostics = cli.getDiagnostics();
    if (!diagnostics.empty()) {
        // CLI error
        diagnostics::DiagnosticPrinter printer;
        printer.printDiagnostics(diagnostics, io::getStdErr());
        return 1;
    }

	VEE_ASSERT(invocation.isValid(), "Invalid invocation after parsing and validation");
    return cli.dispatch(invocation);
}

VEEC_NAMESPACE_END
