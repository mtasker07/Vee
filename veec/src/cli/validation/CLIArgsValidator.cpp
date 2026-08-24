#include "veec/cli/validation/CLIArgsValidator.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/CLIContext.hpp"
#include "veec/cli/CLI.hpp"
#include "veec/cli/CLICommand.hpp"
#include "veec/cli/CLIOption.hpp"
#include "veec/cli/CLIValue.hpp"
#include "veec/cli/CLICommandOptions.hpp"
#include "veec/cli/descriptor/CLICommandDescriptor.hpp"
#include "veec/cli/descriptor/CLIOptionDescriptor.hpp"
#include "veec/cli/descriptor/CLIRootDescriptor.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace validation {

CLIInvocation CLIArgsValidator::validateAndGenerateInvocation() {
    _resultCommand = _parseResult.getCommand();
    _resultOptions = {};

    validatePositionals();
    for (const parsing::CLIArgsParsedOption& parsedOption : _parseResult.getOptions()) {
        validateParsedOption(parsedOption);
    }

    validateAll();
    return CLIInvocation(_resultCommand, _resultOptions);
}

void CLIArgsValidator::validatePositionals() {
    for (std::string_view positional : _parseResult.getPositionals()) {
        _resultOptions.addPositional(positional);
    }
}
void CLIArgsValidator::validateParsedOption(const parsing::CLIArgsParsedOption& parsedOption) {
    const descriptor::CLIOptionDescriptor* descriptor
        = descriptor::getOptionDescriptor(parsedOption.option);

    VEE_ASSERT(descriptor != nullptr, "Parsed option has no corresponding descriptor");

    switch (descriptor->type) {
        case CLIOptionType::Flag:
            validateFlag(parsedOption, *descriptor);
            return;

        case CLIOptionType::Counter:
            validateCounter(parsedOption, *descriptor);
            return;

        case CLIOptionType::Value:
            validateSingleValue(parsedOption, *descriptor);
            return;

        case CLIOptionType::List:
            validateListValues(parsedOption, *descriptor);
            return;

        default:
            VEE_UNREACHABLE("Invalid CLIOptionType");
    }
}
void CLIArgsValidator::validateFlag(const parsing::CLIArgsParsedOption& parsedOption, const descriptor::CLIOptionDescriptor& descriptor) {
    // Duplicate flag
    if (_resultOptions.isOptionSpecified(descriptor.option)) {
        _ctx.diagnostics.report(
            diagnostics::ERROR_CLI_OPTION_DUPLICATE,
            _args.getArgRange(parsedOption.argIndex),
            descriptor::getOptionFullName(descriptor)
        );
        return;
    }

    _resultOptions.setFlag(descriptor.option);
}
void CLIArgsValidator::validateCounter(const parsing::CLIArgsParsedOption& parsedOption, const descriptor::CLIOptionDescriptor& descriptor) {
    // TODO
    (void)parsedOption;
    _resultOptions.incrementCounter(descriptor.option);
}
void CLIArgsValidator::validateSingleValue(const parsing::CLIArgsParsedOption& parsedOption, const descriptor::CLIOptionDescriptor& descriptor) {
    // Duplicate value
    if (_resultOptions.isOptionSpecified(descriptor.option)) {
        _ctx.diagnostics.report(
            diagnostics::ERROR_CLI_OPTION_DUPLICATE,
            _args.getArgRange(parsedOption.argIndex),
            descriptor::getOptionFullName(descriptor)
        );
        return;
    }

    // Check number of values passed
    if (parsedOption.values.empty()) {
        _ctx.diagnostics.report(
            diagnostics::ERROR_CLI_OPTION_EXPECTS_MIN_VALUES,
            _args.getArgRange(parsedOption.argIndex),
            descriptor::getOptionFullName(descriptor),
            1
        );
        
        // Mark option *was* specified
        _resultOptions.setValue(descriptor.option, CLIValue());
        return;
    }

    VEE_ASSERT(parsedOption.values.size() == 1, "Single-value option must have exactly one value");

    const CLIValue& value = parsedOption.values.front();
    validateValueType(value, parsedOption, descriptor);

    // Option validation delegate
    if (descriptor.validateValue) {
        descriptor::CLIOptionValidationContext ctx{
            _ctx.diagnostics,
            _args.getArgRange(parsedOption.argIndex),
            descriptor
        };
        descriptor.validateValue(ctx, value);
    }

    // Always set despite validation above, we dont want further diagnostics
    // if it thinks this value wasnt specified at all.
	_resultOptions.setValue(descriptor.option, value);
}
void CLIArgsValidator::validateListValues(const parsing::CLIArgsParsedOption& parsedOption, const descriptor::CLIOptionDescriptor& descriptor) {
    if (parsedOption.values.empty()) {
        // Invalid, but we should still add an empty list to indicate the option
        // was partially specified.
        _resultOptions.addEmptyList(descriptor.option);
        return;
    }
    
    for (const CLIValue& value : parsedOption.values) {
        // We dont use the retun values of any validation methods below as we
        // still want to add broken values to the result to indicate it was specified.
        // The validator user should check for diagnostics to see if validation failed
        // and prevent actual usage.
        validateValueType(value, parsedOption, descriptor);

        // Option validation delegate
        if (descriptor.validateValue) {
            descriptor::CLIOptionValidationContext ctx{
                _ctx.diagnostics,
                _args.getArgRange(parsedOption.argIndex),
                descriptor
            };
            descriptor.validateValue(ctx, value);
        }

        _resultOptions.addListValue(descriptor.option, value);
    }
}

bool CLIArgsValidator::validateValueType(const CLIValue& value, const parsing::CLIArgsParsedOption& parsedOption, const descriptor::CLIOptionDescriptor& descriptor) {
    VEE_ASSERT(descriptor.valueType != CLIValueType::None, "Descriptor value type cannot be None");

    if (value.canBe(descriptor.valueType)) {
        return true;
    }

    _ctx.diagnostics.report(
        diagnostics::ERROR_CLI_OPTION_VALUE_TYPE_MISMATCH,
        _args.getArgRange(parsedOption.argIndex),
        value.getStringValue(),
        toString(descriptor.valueType),
        descriptor::getOptionFullName(descriptor)
    );
    return false;
}
void CLIArgsValidator::validateAll() {
    // Global validation is validation over all descriptors to ensure options
    // specified/not specified are valid.
    const descriptor::CLICommandDescriptor* commandDesc
        = descriptor::getCommandDescriptor(_parseResult.getCommand());

    auto validateAllList = [&](const std::vector<descriptor::CLIOptionDescriptor>& descriptors) {
        for (const descriptor::CLIOptionDescriptor& descriptor : descriptors) {
            // Required options
            if (descriptor.required && !_resultOptions.isOptionSpecified(descriptor.option)) {
                _ctx.diagnostics.report(
                    diagnostics::ERROR_CLI_OPTION_REQUIRED,
                    _args.getGlobalRange(),
                    descriptor::getOptionFullName(descriptor)
                );
            }

            // Min values
            if (descriptor.type == CLIOptionType::List) {
                size_t valueCount = _resultOptions.getListValues(descriptor.option).size();
                if (valueCount < static_cast<size_t>(descriptor.minValues)) {
                    _ctx.diagnostics.report(
                        diagnostics::ERROR_CLI_OPTION_EXPECTS_MIN_VALUES,
                        _args.getGlobalRange(),
                        descriptor::getOptionFullName(descriptor),
                        descriptor.minValues
                    );
                }
            }

            // Max values
            if (descriptor.type == CLIOptionType::List) {
                size_t valueCount = _resultOptions.getListValues(descriptor.option).size();
                if (descriptor.maxValues >= 0 && valueCount > static_cast<size_t>(descriptor.maxValues)) {
                    _ctx.diagnostics.report(
                        diagnostics::ERROR_CLI_OPTION_EXPECTS_MAX_VALUES,
                        _args.getGlobalRange(),
                        descriptor::getOptionFullName(descriptor),
                        descriptor.maxValues
                    );
                }
            }
        }
    };

	// Validate command-specific options
	if (commandDesc != nullptr) {
        validateAllList(commandDesc->options);
    }
	// Validate global options
    validateAllList(descriptor::getRootDescriptor().globalOptions);
}

} // namespace validation
} // namespace cli
VEEC_NAMESPACE_END
