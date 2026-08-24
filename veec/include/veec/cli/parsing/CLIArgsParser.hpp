/**
 * @file CLIArgsParser.hpp
 * @brief This file contains the definition of the CLIArgsParser class.
 * 
 * The CLIArgsParser class is used for parsing command-line arguments.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/CLIContext.hpp"
#include "veec/cli/CLICommand.hpp"
#include "veec/cli/CLIOption.hpp"
#include "veec/cli/CLIValue.hpp"
#include "veec/cli/CLIArgsToken.hpp"
#include "veec/cli/descriptor/CLIDescriptorFwd.hpp"
#include "veec/cli/parsing/CLIArgsParseResult.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

// cli Forward
class CLIRawArgs;

namespace parsing {

struct CLIArgsParserState {
    u32 currentTokenIndex = 0;
    CLICommand command = CLICommand::Unknown;
    CLIArgsParseResult result = {};
};

class CLIArgsParser {
public:
    CLIArgsParser(CLIContext& ctx, const CLIRawArgs& args, const std::vector<CLIArgsToken>& tokens)
        : _ctx(ctx), _args(args), _tokens(tokens) {}

    CLIArgsParseResult parse();

private:
    CLIContext& _ctx;
    const CLIRawArgs& _args;
    const std::vector<CLIArgsToken>& _tokens;
    CLIArgsParserState _state;

    void parseCommand();
    void parsePositional();
    CLIValue parseValue();
    void parseLongOption();
    void parseShortOption();
    void parseShortSequence();

    inline const CLIArgsToken& peek() {
        return _tokens[_state.currentTokenIndex];
    }
    inline const CLIArgsToken* advance() {
        if (!isAtEnd()) {
            return &_tokens[_state.currentTokenIndex++];
        }
        return nullptr;
    }
    inline const CLIArgsToken* match(CLIArgsTokenType type) {
        if (isAtEnd()) {
            return nullptr;
        }
        const CLIArgsToken& token = peek();
        if (token.type == type) {
            ++_state.currentTokenIndex;
            return &token;
        }
        return nullptr;
    }
    inline bool isAtEnd() const {
        return _tokens[_state.currentTokenIndex].type == CLIArgsTokenType::EndOfArgs;
    }
};

} // namespace parsing
} // namespace cli
VEEC_NAMESPACE_END
