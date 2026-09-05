#include "veec/cli/commands/Compile.hpp"

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/Compilation.hpp"
#include "veec/compilation/CompilationConfig.hpp"
#include "veec/io/StdStreams.hpp"
#include "veec/fs/Path.hpp"
#include "veec/cli/CLICommand.hpp"
#include "veec/cli/CLIOption.hpp"
#include "veec/cli/CLIOptionType.hpp"
#include "veec/cli/CLIValue.hpp"
#include "veec/cli/descriptor/CLIDescriptorFwd.hpp"
#include "veec/cli/descriptor/CLICommandDescriptor.hpp"
#include "veec/cli/descriptor/CLIOptionDescriptor.hpp"
#include "veec/cli/delegate/CLIDelegateTypes.hpp"
#include "veec/cli/delegate/CLIOptionValidationDelegates.hpp"
#include "veec/diagnostics/UserDiagnostic.hpp"
#include "veec/diagnostics/DiagnosticPrinter.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace commands {

namespace {

compilation::CompilationConfig generateCompilationConfigFromOptions(const CLICommandOptions& options) {
    compilation::CompilationConfig config;

    config.inputFiles = options.getListValuesAsPaths(CLIOption::InputFile);
    config.outputFile = options.getValueAsPathOr(CLIOption::OutputFile, fs::Path("a.out"));
    config.outputMirDirectory = options.getValueAsPathOr(CLIOption::OutputMir, fs::Path());
    // TODO: Optimization level etc.
    config.backendIdentifier = options.getValueAsStringOr(CLIOption::Backend, "default");

    return config;
}

} // namespace

const std::vector<descriptor::CLIOptionDescriptor>& getCompileCommandOptionDescriptors() {
    static const std::vector<descriptor::CLIOptionDescriptor> compileCommandOptionDescriptors = {
        // INPUT FILE(S)

        {
            /* option               */ CLIOption::InputFile,
            /* nameShort            */ 'i',
            /* nameLong             */ "input",
            /* description          */ "Input file(s)",
            /* type                 */ CLIOptionType::List,
            /* valueType            */ CLIValueType::String,
            /* defaultValue         */ {},
            /* flagDefaultValue     */ false,
            /* minValues            */ 1,
            /* maxValues            */ -1,
            /* required             */ true,
            /* validateValue        */ delegate::inputFileValidationDelegate,
        },

        // OUTPUT FILE

        {
            /* option               */ CLIOption::OutputFile,
            /* nameShort            */ 'o',
            /* nameLong             */ "output-file",
            /* description          */ "Output file",
            /* type                 */ CLIOptionType::Value,
            /* valueType            */ CLIValueType::String,
            /* defaultValue         */ {},
            /* flagDefaultValue     */ false,
            /* minValues            */ 1,
            /* maxValues            */ 1,
            /* required             */ true,
            /* validateValue        */ delegate::outputFileValidationDelegate,
        },

        // OPTIMIZATION LEVEL

        {
            /* option               */ CLIOption::OptimizationLevel,
            /* nameShort            */ 'O',
            /* nameLong             */ "opt-level",
            /* description          */ "Optimization level",
            /* type                 */ CLIOptionType::Value,
            /* valueType            */ CLIValueType::Integer,
            /* defaultValue         */ {},
            /* flagDefaultValue     */ false,
            /* minValues            */ 1,
            /* maxValues            */ 1,
            /* required             */ false,
            /* validateValue        */ delegate::optimizationLevelValidationDelegate,
        },

        // OUTPUT MIR

        {
            /* option               */ CLIOption::OutputMir,
            /* nameShort            */ '\0',
            /* nameLong             */ "output-mir",
            /* description          */ "Output MIR to directory",
            /* type                 */ CLIOptionType::Value,
            /* valueType            */ CLIValueType::String,
            /* defaultValue         */ {},
            /* flagDefaultValue     */ false,
            /* minValues            */ 1,
            /* maxValues            */ 1,
            /* required             */ false,
            /* validateValue        */ delegate::mirOutputDirectoryValidationDelegate,
        },

        // BACKEND

        {
            /* option               */ CLIOption::Backend,
            /* nameShort            */ '\0',
            /* nameLong             */ "backend",
            /* description          */ "Backend to use for code generation",
            /* type                 */ CLIOptionType::Value,
            /* valueType            */ CLIValueType::String,
            /* defaultValue         */ CLIValue("default"),
            /* flagDefaultValue     */ false,
            /* minValues            */ 1,
            /* maxValues            */ 1,
            /* required             */ false,
            /* validateValue        */ nullptr, // Not sure how to approach this yet
        },
    };

    return compileCommandOptionDescriptors;
}

const descriptor::CLICommandDescriptor& getCompileCommandDescriptor() {
    static const descriptor::CLICommandDescriptor compileCommandDescriptor = {
        // command
        CLICommand::Compile,
        // subcommands
        {},
        // namePrimary
        "compile",
        // nameAliases
        {},
        // description
        "Compile source file(s) into an executable",
        // options
        getCompileCommandOptionDescriptors(),
        // invocationDelegate
        getCompileCommandInvocationDelegate()
    };

    return compileCommandDescriptor;
}
const delegate::CLICommandInvocationDelegate& getCompileCommandInvocationDelegate() {
    static const delegate::CLICommandInvocationDelegate compileCommandInvocationDelegate
        = [](descriptor::CLICommandInvocationContext&, const CLICommandOptions& options) -> int
    {
        compilation::CompilationConfig config = generateCompilationConfigFromOptions(options);
        compilation::Compilation compilation = compilation::Compilation(config);
        compilation.compile();

        const std::vector<diagnostics::UserDiagnostic>& diagnostics = compilation.getDiagnostics();
        if (diagnostics.empty()) {
            return 0; // Success
        }

        // Print generated diagnostics
        diagnostics::DiagnosticPrinter().printDiagnostics(diagnostics, io::getStdErr());

        return 1; // Failure
    };

    return compileCommandInvocationDelegate;
}

} // namespace commands
} // namespace cli
VEEC_NAMESPACE_END
