#include "veec/cli/parsing/CLIArgsParser.hpp"

#include <utility>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/cli/CLIContext.hpp"
#include "veec/cli/CLIOption.hpp"
#include "veec/cli/CLIValue.hpp"
#include "veec/cli/CLIArgsToken.hpp"
#include "veec/cli/descriptor/CLIRootDescriptor.hpp"
#include "veec/cli/descriptor/CLICommandDescriptor.hpp"
#include "veec/cli/descriptor/CLIOptionDescriptor.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace parsing {

CLIArgsParseResult CLIArgsParser::parse() {
    _state = CLIArgsParserState();

    parseCommand();
    _state.result.setCommand(_state.command);

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

    return _state.result;
}

void CLIArgsParser::parseCommand() {
    const descriptor::CLIRootDescriptor& rootDesc
        = descriptor::getRootDescriptor();

    // TODO: Use this maybe?
    std::vector<std::string_view> commandPathUsed;

    auto unknownCommand = [&](const CLIArgsToken& token) {
        _ctx.diagnostics.report(
            diagnostics::ERROR_CLI_UNKNOWN_COMMAND,
            _args.getArgRange(token.argvIndex),
            token.lexeme
        );
        _state.command = CLICommand::Unknown;
    };
    auto assumeDefaultCommand = [&]() {
        _state.command = rootDesc.commands[rootDesc.defaultCommandIndex].command;
    };

    if (isAtEnd() || peek().type != CLIArgsTokenType::Positional) {
        assumeDefaultCommand();
        return;
    }

    // Search command tree for each positional token
    const descriptor::CLICommandDescriptor* currentCommandDesc = nullptr;
    while (peek().type == CLIArgsTokenType::Positional) {
        const CLIArgsToken& token = *advance();

        // Add to command path used
        commandPathUsed.push_back(token.lexeme);

        // Lookup in current command (or root if no current command)
        currentCommandDesc = descriptor::getCommandDescriptorByName(
            token.lexeme,
            currentCommandDesc,
            true // << include aliases
        );

        if (currentCommandDesc == nullptr) {
            unknownCommand(token);
            return;
        }
    }

    _state.command = currentCommandDesc->command;
}
void CLIArgsParser::parsePositional() {
    _state.result.addPositional(advance()->lexeme);
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

    const descriptor::CLIOptionDescriptor* descriptor
        = descriptor::getOptionDescriptorByLongName(_state.command, token.optionName);
        
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
        _state.result.addOption(descriptor->option, token.optionName, token.argvIndex);
        return;
    }
    else if (descType == CLIOptionType::Counter) {
        _state.result.addOption(descriptor->option, token.optionName, token.argvIndex);
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
        _state.result.addOption(descriptor->option, token.optionName, token.argvIndex, std::move(values));
        return;
    }
}
void CLIArgsParser::parseShortOption() {
    const CLIArgsToken& token = *advance();

    VEE_ASSERT(!token.optionName.empty(), "Short option token must have a name");
    VEE_ASSERT(token.optionName.size() == 1, "Short option token must have a single character name");

    const descriptor::CLIOptionDescriptor* descriptor
        = descriptor::getOptionDescriptorByShortName(_state.command, token.optionName[0]);
        
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
        _state.result.addOption(descriptor->option, token.optionName, token.argvIndex);
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
        _state.result.addOption(descriptor->option, token.optionName, token.argvIndex, std::move(values));
        return;
    }
}
void CLIArgsParser::parseShortSequence() {
    const CLIArgsToken& token = *advance();

    VEE_ASSERT(!token.optionName.empty(), "Short sequence token must have a name");
    VEE_ASSERT(token.optionName.size() > 1, "Short sequence token must have multiple characters");

    for (size_t i = 0; i < token.optionName.size(); ++i) {
        char c = token.optionName[i];

        const descriptor::CLIOptionDescriptor* descriptor
            = descriptor::getOptionDescriptorByShortName(_state.command, c);
            
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
            _state.result.addOption(descriptor->option, std::string_view(&c, 1), token.argvIndex);
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

            _state.result.addOption(descriptor->option, std::string_view(&c, 1), token.argvIndex, std::move(values));
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

            _state.result.addOption(descriptor->option, std::string_view(&c, 1), token.argvIndex, std::move(values));
            break;
        }
    }
}

} // namespace parsing
} // namespace cli
VEEC_NAMESPACE_END
