/**
 * @file CLIArgsLexer.hpp
 * @brief This file contains the definition of the CLIArgsLexer class.
 * 
 * The CLIArgsLexer class is used for tokenizing command-line arguments.
 */

#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/cli/CLIContext.hpp"
#include "veec/cli/args/CLIArgs.hpp"
#include "veec/cli/args/CLIArgsToken.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace args {
namespace lexing {

class CLIArgsLexer {
public:
    CLIArgsLexer(compilation::CompilationContext& ctx, const CLIArgs& args)
        : _ctx(ctx), _args(args) {}

    std::vector<CLIArgsToken> tokenize();

private:
    compilation::CompilationContext& _ctx;
    const CLIArgs& _args;
    size_t _currentArgIndex = 0;
    std::vector<CLIArgsToken> _tokens;
    bool _endOfOptionsEncountered = false;

    void scanArg(std::string_view arg);

    void emitToken(CLIArgsTokenType type, std::string_view lexeme, std::string_view optionName = {});

    bool isAtEnd() const;
};

} // namespace lexing
} // namespace args
} // namespace cli
VEEC_NAMESPACE_END
