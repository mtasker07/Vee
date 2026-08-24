#include "veec/cli/CLI.hpp"

#include <vector>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/CLIRawArgs.hpp"
#include "veec/cli/CLIOption.hpp"
#include "veec/cli/CLICommand.hpp"
#include "veec/cli/commands/Compile.hpp"
#include "veec/cli/descriptor/CLIDescriptorFwd.hpp"
#include "veec/cli/descriptor/CLIRootDescriptor.hpp"
#include "veec/cli/descriptor/CLICommandDescriptor.hpp"
#include "veec/cli/descriptor/CLIOptionDescriptor.hpp"
#include "veec/cli/lexing/CLIArgsLexer.hpp"
#include "veec/cli/parsing/CLIArgsParser.hpp"
#include "veec/cli/validation/CLIArgsValidator.hpp"

// Commands
#include "veec/cli/commands/Compile.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

CLIInvocation CLI::parse(int argc, const char** argv) {
	_ctx.args = CLIRawArgs(argc, argv);
    
    // Lex
    lexing::CLIArgsLexer lexer(_ctx, _ctx.args);
    std::vector<CLIArgsToken> tokens = lexer.tokenize();

    // Parse
    parsing::CLIArgsParser parser(_ctx, _ctx.args, tokens);
    parsing::CLIArgsParseResult parseResult = parser.parse();

    // Validate
    validation::CLIArgsValidator validator(_ctx, _ctx.args, parseResult);
    return validator.validateAndGenerateInvocation();
}

int CLI::dispatch(const CLIInvocation& invocation) {
    VEE_ASSERT(invocation.isValid(), "Cannot dispatch invalid invocation");

    // Invoke invocation delegate
    const descriptor::CLICommandDescriptor* commandDescriptor
        = descriptor::getCommandDescriptor(invocation.getCommand());

    VEE_ASSERT(commandDescriptor != nullptr, "Command descriptor not found");
    VEE_ASSERT(commandDescriptor->invocationDelegate != nullptr,
        "Command descriptor has no invocation delegate");

    descriptor::CLICommandInvocationContext ctx {
        *commandDescriptor
    };
    return commandDescriptor->invocationDelegate(ctx, invocation.getOptions());
}

} // namespace cli
VEEC_NAMESPACE_END
