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
#include "veec/cli/CLIContext.hpp"
#include "veec/cli/CLIRawArgs.hpp"
#include "veec/cli/CLIArgsToken.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace lexing {

struct CLIArgsLexerState {
    size_t currentArgIndex = 0;
    bool endOfOptionsEncountered = false;
    std::vector<CLIArgsToken> tokens = {};
};

class CLIArgsLexer {
public:
    CLIArgsLexer(CLIContext& ctx, const CLIRawArgs& args)
        : _ctx(ctx), _args(args) {}

    std::vector<CLIArgsToken> tokenize();

private:
    CLIContext& _ctx;
    const CLIRawArgs& _args;
    CLIArgsLexerState _state;

    void scanArg(std::string_view arg);

    void emitToken(CLIArgsTokenType type, std::string_view lexeme, std::string_view optionName = {});

    bool isAtEnd() const;
};

} // namespace lexing
} // namespace cli
VEEC_NAMESPACE_END
