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
#include "veec/cli/args/CLIOption.hpp"
#include "veec/cli/args/CLIValue.hpp"
#include "veec/cli/args/CLIArgsToken.hpp"
#include "veec/cli/args/parsing/CLIArgsParseResult.hpp"

VEEC_NAMESPACE_BEGIN

namespace compilation {
    class CompilationContext;
}

namespace cli {
namespace args {
namespace parsing {

class CLIArgsParser {
public:
    CLIArgsParser(compilation::CompilationContext& ctx, const CLIArgs& args, const std::vector<CLIArgsToken>& tokens)
        : _ctx(ctx), _args(args), _tokens(tokens) {}

    CLIArgsParseResult parse();

private:
    compilation::CompilationContext& _ctx;
    const CLIArgs& _args;
    const std::vector<CLIArgsToken>& _tokens;
    u32 _currentTokenIndex = 0;
    CLIArgsParseResult _result;

    void parsePositional();
    CLIValue parseValue();
    void parseLongOption();
    void parseShortOption();
    void parseShortSequence();

    const CLIOptionDescriptor* findOptionDescriptorByLongName(std::string_view longName);
    const CLIOptionDescriptor* findOptionDescriptorByShortName(char shortName);

    inline const CLIArgsToken& peek() {
        return _tokens[_currentTokenIndex];
    }
    inline const CLIArgsToken* advance() {
        if (!isAtEnd()) {
            return &_tokens[_currentTokenIndex++];
        }
        return nullptr;
    }
    inline const CLIArgsToken* match(CLIArgsTokenType type) {
        if (isAtEnd()) {
            return nullptr;
        }
        const CLIArgsToken& token = peek();
        if (token.type == type) {
            ++_currentTokenIndex;
            return &token;
        }
        return nullptr;
    }
    inline bool isAtEnd() const {
        return _tokens[_currentTokenIndex].type == CLIArgsTokenType::EndOfArgs;
    }
};

} // namespace parsing
} // namespace args
} // namespace cli
VEEC_NAMESPACE_END
