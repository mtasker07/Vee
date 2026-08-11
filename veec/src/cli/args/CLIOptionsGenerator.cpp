#include "veec/cli/args/CLIOptionsGenerator.hpp"

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/cli/args/CLIOptions.hpp"
#include "veec/cli/args/CLIArgs.hpp"
#include "veec/cli/args/CLIArgsToken.hpp"
#include "veec/cli/args/lexing/CLIArgsLexer.hpp"
#include "veec/cli/args/parsing/CLIArgsParser.hpp"
#include "veec/cli/args/parsing/CLIArgsParseResult.hpp"
#include "veec/cli/args/validation/CLIArgsValidator.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace args {

CLIOptions CLIOptionsGenerator::generateFromArgs(const CLIArgs& args) {
    // Lex
    lexing::CLIArgsLexer lexer(_ctx, args);
    std::vector<CLIArgsToken> tokens = lexer.tokenize();

    // Parse
    parsing::CLIArgsParser parser(_ctx, args, tokens);
    parsing::CLIArgsParseResult parseResult = parser.parse();

    // Validate
    validation::CLIArgsValidator validator(_ctx, args, parseResult);
    return validator.validate();
}

} // namespace args
} // namespace cli
VEEC_NAMESPACE_END
