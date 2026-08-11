#include "veec/cli/args/lexing/CLIArgsLexer.hpp"

#include <string_view>
#include <vector>
#include <cctype>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/cli/args/CLIArgsToken.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace args {
namespace lexing {

std::vector<CLIArgsToken> CLIArgsLexer::tokenize() {
    _currentArgIndex = 0;
    _endOfOptionsEncountered = false;
    _tokens.clear();
    
    while (!isAtEnd()) {
        scanArg(_args.getArgValue(_currentArgIndex));
        ++_currentArgIndex;
    }

    emitToken(CLIArgsTokenType::EndOfArgs, "EndOfArgs");

    return _tokens;
}

void CLIArgsLexer::scanArg(std::string_view arg) {
    if (!_endOfOptionsEncountered && arg == "--") {
        _endOfOptionsEncountered = true;
        emitToken(CLIArgsTokenType::EndOfOptions, arg);
        return;
    }

    if (_endOfOptionsEncountered || arg.empty() || arg == "-") {
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
    _tokens.push_back(CLIArgsToken{ type, lexeme, _currentArgIndex + 1, optionName });
    // _currentArgIndex + 1 because _args does not include program path ^^
    // however, the argIndex in the token should be the argvIndex.
}

bool CLIArgsLexer::isAtEnd() const {
    return _currentArgIndex >= _args.getArgCount();
}

} // namespace lexing
} // namespace args
} // namespace cli
VEEC_NAMESPACE_END
