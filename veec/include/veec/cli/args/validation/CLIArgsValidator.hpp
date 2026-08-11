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
#include "veec/cli/args/CLIOption.hpp"
#include "veec/cli/args/CLIValue.hpp"
#include "veec/cli/args/CLIOptions.hpp"
#include "veec/cli/args/parsing/CLIArgsParseResult.hpp"

VEEC_NAMESPACE_BEGIN

namespace compilation {
    class CompilationContext;
}

namespace cli {
namespace args {
namespace validation {

class CLIArgsValidator {
public:
    CLIArgsValidator(compilation::CompilationContext& ctx, const CLIArgs& args, const parsing::CLIArgsParseResult& parseResult)
        : _ctx(ctx), _args(args), _parseResult(parseResult) {}

    CLIOptions validate();

private:
    compilation::CompilationContext& _ctx;
    const CLIArgs& _args;
    const parsing::CLIArgsParseResult& _parseResult;
    CLIOptions _result;

    void validatePositionals();
    void validateParsedOption(const parsing::CLIArgsParsedOption& parsedOption);
    void validateFlag(const parsing::CLIArgsParsedOption& parsedOption, const CLIOptionDescriptor& descriptor);
    void validateCounter(const parsing::CLIArgsParsedOption& parsedOption, const CLIOptionDescriptor& descriptor);
    void validateSingleValue(const parsing::CLIArgsParsedOption& parsedOption, const CLIOptionDescriptor& descriptor);
    void validateListValues(const parsing::CLIArgsParsedOption& parsedOption, const CLIOptionDescriptor& descriptor);
    bool validateValueType(const CLIValue& value, const parsing::CLIArgsParsedOption& parsedOption, const CLIOptionDescriptor& descriptor);
    void validateGlobal();
};

} // namespace validation
} // namespace args
} // namespace cli
VEEC_NAMESPACE_END
