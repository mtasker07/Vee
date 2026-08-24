#include "veec/cli/lexing/CLIArgsLexer.hpp"

#include <string_view>
#include <vector>
#include <utility>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/cli/CLIArgsToken.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace lexing {

std::vector<CLIArgsToken> CLIArgsLexer::tokenize() {
    _state = CLIArgsLexerState();
    
    while (!isAtEnd()) {
        scanArg(_args.getArgValue(_state.currentArgIndex));
        ++_state.currentArgIndex;
    }

    emitToken(CLIArgsTokenType::EndOfArgs, "EndOfArgs");

    return std::move(_state.tokens);
}

void CLIArgsLexer::scanArg(std::string_view arg) {
    if (!_state.endOfOptionsEncountered && arg == "--") {
        _state.endOfOptionsEncountered = true;
        emitToken(CLIArgsTokenType::EndOfOptions, arg);
        return;
    }

    if (_state.endOfOptionsEncountered || arg.empty() || arg == "-") {
        emitToken(CLIArgsTokenType::Positional, arg);
        return;
    }

    if (arg.size() > 2 && arg[0] == '-' && arg[1] == '-') {
        emitToken(CLIArgsTokenType::LongOption, arg, arg.substr(2));
        return;
    }

    if (arg.size() > 1 && arg[0] == '-') {
        // Single short option (e.g., -o)
        if (arg.size() == 2) {
            emitToken(CLIArgsTokenType::ShortOption, arg, arg.substr(1, 1));
            return;
        }

        // Short sequence (e.g., -abc, -O2)
        if (arg.size() > 2) {
            emitToken(CLIArgsTokenType::ShortSequence, arg, arg.substr(1));
            return;
        }
    }

    emitToken(CLIArgsTokenType::Positional, arg);
}

void CLIArgsLexer::emitToken(CLIArgsTokenType type, std::string_view lexeme, std::string_view optionName) {
    _state.tokens.push_back(CLIArgsToken{ type, lexeme, _state.currentArgIndex + 1, optionName });
    // _state.currentArgIndex + 1 because _args does not include program path ^^
    // however, the argIndex in the token should be the argvIndex.
}

bool CLIArgsLexer::isAtEnd() const {
    return _state.currentArgIndex >= _args.getArgCount();
}

} // namespace lexing
} // namespace cli
VEEC_NAMESPACE_END
