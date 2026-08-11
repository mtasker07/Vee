#include "veec/cli/args/validation/CLIArgsValidator.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace args {
namespace validation {

CLIOptions CLIArgsValidator::validate() {
    _result = {};

    validatePositionals();
    for (const parsing::CLIArgsParsedOption& parsedOption : _parseResult.getOptions()) {
        validateParsedOption(parsedOption);
    }

    validateGlobal();
    return _result;
}

void CLIArgsValidator::validatePositionals() {
    for (std::string_view positional : _parseResult.getPositionals()) {
        _result.addPositional(positional);
    }
}
void CLIArgsValidator::validateParsedOption(const parsing::CLIArgsParsedOption& parsedOption) {
    const CLIOptionDescriptor* descriptor = getOptionDescriptor(parsedOption.option);
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
void CLIArgsValidator::validateFlag(const parsing::CLIArgsParsedOption& parsedOption, const CLIOptionDescriptor& descriptor) {
    // Duplicate flag
    if (_result.isOptionSpecified(descriptor.option)) {
        _ctx.diagnostics.report(
            diagnostics::ERROR_CLI_OPTION_DUPLICATE,
            _args.getArgRange(parsedOption.argIndex),
            getOptionFullName(parsedOption.option)
        );
        return;
    }

    _result.setFlag(descriptor.option);
}
void CLIArgsValidator::validateCounter(const parsing::CLIArgsParsedOption& parsedOption, const CLIOptionDescriptor& descriptor) {
    // TODO
    (void)parsedOption;
    _result.incrementCounter(descriptor.option);
}
void CLIArgsValidator::validateSingleValue(const parsing::CLIArgsParsedOption& parsedOption, const CLIOptionDescriptor& descriptor) {
    // Duplicate value
    if (_result.isOptionSpecified(descriptor.option)) {
        _ctx.diagnostics.report(
            diagnostics::ERROR_CLI_OPTION_DUPLICATE,
            _args.getArgRange(parsedOption.argIndex),
            getOptionFullName(parsedOption.option)
        );
        return;
    }

    // Check number of values passed
    if (parsedOption.values.empty()) {
        _ctx.diagnostics.report(
            diagnostics::ERROR_CLI_OPTION_EXPECTS_MIN_VALUES,
            _args.getArgRange(parsedOption.argIndex),
            getOptionFullName(parsedOption.option),
            1
        );
        // Still set the value to indicate it *was* specified just in an
        // invalid state
        _result.setValue(descriptor.option, CLIValue());
        return;
    }

    VEE_ASSERT(parsedOption.values.size() == 1, "Single-value option must have exactly one value");

    const CLIValue& value = parsedOption.values.front();
    if (!validateValueType(value, parsedOption, descriptor)) {
        return;
    }

    _result.setValue(descriptor.option, value);
}
void CLIArgsValidator::validateListValues(const parsing::CLIArgsParsedOption& parsedOption, const CLIOptionDescriptor& descriptor) {
    if (parsedOption.values.empty()) {
        // Invalid, but we should still add an empty list to indicate the option
        // was partially specified.
        _result.addEmptyList(descriptor.option);
        return;
    }
    
    for (const CLIValue& value : parsedOption.values) {
        if (!validateValueType(value, parsedOption, descriptor)) {
            continue;
        }

        _result.addListValue(descriptor.option, value);
    }
}

bool CLIArgsValidator::validateValueType(const CLIValue& value, const parsing::CLIArgsParsedOption& parsedOption, const CLIOptionDescriptor& descriptor) {
    VEE_ASSERT(descriptor.valueType != CLIValueType::None, "Descriptor value type cannot be None");

    if (value.canBe(descriptor.valueType)) {
        return true;
    }

    _ctx.diagnostics.report(
        diagnostics::ERROR_CLI_OPTION_VALUE_TYPE_MISMATCH,
        _args.getArgRange(parsedOption.argIndex),
        value.getStringValue(),
        toString(descriptor.valueType),
        getOptionFullName(descriptor.option)
    );
    return false;
}
void CLIArgsValidator::validateGlobal() {
    // Global validation is validation over all descriptors to ensure options
    // specified/not specified are valid.
    for (size_t i = 0; i < CLI_OPTION_DESCRIPTOR_COUNT; ++i) {
        const CLIOptionDescriptor& descriptor = CLI_OPTION_DESCRIPTORS[i];

        // Required options
        if (descriptor.required && !_result.isOptionSpecified(descriptor.option)) {
            _ctx.diagnostics.report(
                diagnostics::ERROR_CLI_OPTION_REQUIRED,
                _args.getGlobalRange(),
                getOptionFullName(descriptor.option)
            );
        }

        // Min values
        if (descriptor.type == CLIOptionType::List) {
            size_t valueCount = _result.getListValues(descriptor.option).size();
            if (valueCount < static_cast<size_t>(descriptor.minValues)) {
                _ctx.diagnostics.report(
                    diagnostics::ERROR_CLI_OPTION_EXPECTS_MIN_VALUES,
                    _args.getGlobalRange(),
                    getOptionFullName(descriptor.option),
                    descriptor.minValues
                );
            }
        }

        // Max values
        if (descriptor.type == CLIOptionType::List) {
            size_t valueCount = _result.getListValues(descriptor.option).size();
            if (descriptor.maxValues >= 0 && valueCount > static_cast<size_t>(descriptor.maxValues)) {
                _ctx.diagnostics.report(
                    diagnostics::ERROR_CLI_OPTION_EXPECTS_MAX_VALUES,
                    _args.getGlobalRange(),
                    getOptionFullName(descriptor.option),
                    descriptor.maxValues
                );
            }
        }
    }
}

} // namespace validation
} // namespace args
} // namespace cli
VEEC_NAMESPACE_END
