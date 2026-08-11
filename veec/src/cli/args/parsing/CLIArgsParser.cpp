#include "veec/cli/args/parsing/CLIArgsParser.hpp"

#include <utility>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/cli/args/CLIOption.hpp"
#include "veec/cli/args/CLIValue.hpp"
#include "veec/cli/args/CLIArgsToken.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace args {
namespace parsing {

CLIArgsParseResult CLIArgsParser::parse() {
    _currentTokenIndex = 0;
    _result = {};

    while (!isAtEnd()) {
        switch (peek().type) {
            case CLIArgsTokenType::Positional:
                parsePositional();
                break;

            case CLIArgsTokenType::LongOption:
                parseLongOption();
                break;

            case CLIArgsTokenType::ShortOption:
                parseShortOption();
                break;

            case CLIArgsTokenType::ShortSequence:
                parseShortSequence();
                break;

            case CLIArgsTokenType::EndOfOptions:
                advance();
                break;

            default:
                VEE_UNREACHABLE("Unexpected token type");
        }
    }

    return _result;
}

void CLIArgsParser::parsePositional() {
    _result.addPositional(advance()->lexeme);
}
CLIValue CLIArgsParser::parseValue() {
    VEE_ASSERT(!isAtEnd(), "Cannot parse value at end of token stream");
    VEE_ASSERT(peek().type == CLIArgsTokenType::Positional, "CLI value token must be positional");
    return { advance()->lexeme };
}
void CLIArgsParser::parseLongOption() {
    // Find descriptor
    const CLIArgsToken& token = *advance();

    VEE_ASSERT(!token.optionName.empty(), "Long option token must have a name");

    const CLIOptionDescriptor* descriptor
        = findOptionDescriptorByLongName(token.optionName);
        
    if (descriptor == nullptr) {
        // Invalid option
        _ctx.diagnostics.report(
            diagnostics::ERROR_CLI_UNKNOWN_OPTION,
            _args.getArgRange(token.argvIndex),
            token.optionName
        );
        return;
    }

    CLIOptionType descType = descriptor->type;
    if (descType == CLIOptionType::Flag) {
        _result.addOption(descriptor->option, token.optionName, token.argvIndex);
        return;
    }
    else if (descType == CLIOptionType::Counter) {
        _result.addOption(descriptor->option, token.optionName, token.argvIndex);
        return;
    }
    else /* CLIOptionType::List || CLIOptionType::Value */ {
        std::vector<CLIValue> values;
        while (!isAtEnd() && peek().type == CLIArgsTokenType::Positional) {
            values.push_back(parseValue());
            // vv Parse only one value for Value options
            if (descType == CLIOptionType::Value) {
                break;
            }
        }
        _result.addOption(descriptor->option, token.optionName, token.argvIndex, std::move(values));
        return;
    }
}
void CLIArgsParser::parseShortOption() {
    const CLIArgsToken& token = *advance();

    VEE_ASSERT(!token.optionName.empty(), "Short option token must have a name");
    VEE_ASSERT(token.optionName.size() == 1, "Short option token must have a single character name");

    const CLIOptionDescriptor* descriptor
        = findOptionDescriptorByShortName(token.optionName[0]);
        
    if (descriptor == nullptr) {
        // Invalid option
        _ctx.diagnostics.report(
            diagnostics::ERROR_CLI_UNKNOWN_OPTION,
            _args.getArgRange(token.argvIndex),
            token.optionName
        );
        return;
    }

    CLIOptionType descType = descriptor->type;
    if (descType == CLIOptionType::Flag || descType == CLIOptionType::Counter) {
        _result.addOption(descriptor->option, token.optionName, token.argvIndex);
        return;
    }
    else /* CLIOptionType::List || CLIOptionType::Value */ {
        std::vector<CLIValue> values;
        while (!isAtEnd() && peek().type == CLIArgsTokenType::Positional) {
            values.push_back(parseValue());
            // vv Parse only one value for Value options
            if (descType == CLIOptionType::Value) {
                break;
            }
        }
        _result.addOption(descriptor->option, token.optionName, token.argvIndex, std::move(values));
        return;
    }
}
void CLIArgsParser::parseShortSequence() {
    const CLIArgsToken& token = *advance();

    VEE_ASSERT(!token.optionName.empty(), "Short sequence token must have a name");
    VEE_ASSERT(token.optionName.size() > 1, "Short sequence token must have multiple characters");

    for (size_t i = 0; i < token.optionName.size(); ++i) {
        char c = token.optionName[i];

        const CLIOptionDescriptor* descriptor
            = findOptionDescriptorByShortName(c);
            
        if (descriptor == nullptr) {
            // Invalid option
            _ctx.diagnostics.report(
                diagnostics::ERROR_CLI_UNKNOWN_OPTION,
                _args.getArgRange(token.argvIndex),
                std::string_view(&c, 1)
            );
            continue;
        }

        CLIOptionType descType = descriptor->type;
        if (descType == CLIOptionType::Flag || descType == CLIOptionType::Counter) {
            _result.addOption(descriptor->option, std::string_view(&c, 1), token.argvIndex);
            continue;
        }
        else if (descType == CLIOptionType::Value) {
            std::vector<CLIValue> values;

            if (i + 1 < token.optionName.size()) {
                std::string_view attachedValue = token.optionName.substr(i + 1);
                values.push_back(CLIValue(attachedValue));
            }
            else if (!isAtEnd() && peek().type == CLIArgsTokenType::Positional) {
                values.push_back(parseValue());
            }

            _result.addOption(descriptor->option, std::string_view(&c, 1), token.argvIndex, std::move(values));
            break;
        }
        else /* CLIOptionType::List */ {
            std::vector<CLIValue> values;

            if (i + 1 < token.optionName.size()) {
                std::string_view attachedValue = token.optionName.substr(i + 1);
                values.push_back(CLIValue(attachedValue));
            }

            while (!isAtEnd() && peek().type == CLIArgsTokenType::Positional) {
                values.push_back(parseValue());
            }

            _result.addOption(descriptor->option, std::string_view(&c, 1), token.argvIndex, std::move(values));
            break;
        }
    }
}

const CLIOptionDescriptor* CLIArgsParser::findOptionDescriptorByLongName(std::string_view longName) {
    for (size_t i = 0; i < CLI_OPTION_DESCRIPTOR_COUNT; ++i) {
        if (CLI_OPTION_DESCRIPTORS[i].nameLong == longName) {
            return &CLI_OPTION_DESCRIPTORS[i];
        }
    }
    return nullptr;
}
const CLIOptionDescriptor* CLIArgsParser::findOptionDescriptorByShortName(char shortName) {
    for (size_t i = 0; i < CLI_OPTION_DESCRIPTOR_COUNT; ++i) {
        if (CLI_OPTION_DESCRIPTORS[i].nameShort == shortName) {
            return &CLI_OPTION_DESCRIPTORS[i];
        }
    }
    return nullptr;
}

} // namespace parsing
} // namespace args
} // namespace cli
VEEC_NAMESPACE_END
