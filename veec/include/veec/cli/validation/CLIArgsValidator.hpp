/**
 * @file CLIArgsValidator.hpp
 * @brief This file contains the definition of the CLIArgsValidator class.
 *
 * The CLIArgsValidator class validates parsed CLI arguments and builds
 * semantic CLI option state.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/CLIContext.hpp"
#include "veec/cli/CLICommand.hpp"
#include "veec/cli/CLICommandOptions.hpp"
#include "veec/cli/CLIInvocation.hpp"
#include "veec/cli/descriptor/CLIDescriptorFwd.hpp"
#include "veec/cli/parsing/CLIArgsParseResult.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

// cli Forward
class CLIValue;
class CLIRawArgs;

namespace validation {

class CLIArgsValidator {
public:
    CLIArgsValidator(CLIContext& ctx, const CLIRawArgs& args, const parsing::CLIArgsParseResult& parseResult)
        : _ctx(ctx), _args(args), _parseResult(parseResult) {}

    CLIInvocation validateAndGenerateInvocation();

private:
    CLIContext& _ctx;
    const CLIRawArgs& _args;
    const parsing::CLIArgsParseResult& _parseResult;
    CLICommand _resultCommand = CLICommand::Unknown;
    CLICommandOptions _resultOptions = {};

    void validatePositionals();
    void validateParsedOption(const parsing::CLIArgsParsedOption& parsedOption);
    void validateFlag(const parsing::CLIArgsParsedOption& parsedOption, const descriptor::CLIOptionDescriptor& descriptor);
    void validateCounter(const parsing::CLIArgsParsedOption& parsedOption, const descriptor::CLIOptionDescriptor& descriptor);
    void validateSingleValue(const parsing::CLIArgsParsedOption& parsedOption, const descriptor::CLIOptionDescriptor& descriptor);
    void validateListValues(const parsing::CLIArgsParsedOption& parsedOption, const descriptor::CLIOptionDescriptor& descriptor);
    bool validateValueType(const CLIValue& value, const parsing::CLIArgsParsedOption& parsedOption, const descriptor::CLIOptionDescriptor& descriptor);
    void validateAll();
};

} // namespace validation
} // namespace cli
VEEC_NAMESPACE_END
