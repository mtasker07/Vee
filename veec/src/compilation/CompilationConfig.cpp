#include "veec/compilation/CompilationConfig.hpp"

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/args/CLIArgs.hpp"
#include "veec/cli/args/CLIOption.hpp"
#include "veec/cli/args/CLIOptions.hpp"

VEEC_NAMESPACE_BEGIN
namespace compilation {

namespace {

std::vector<fs::Path> getPathValues(const cli::args::CLIOptions& options, cli::args::CLIOption option) {
    std::vector<fs::Path> values;
    const std::vector<cli::args::CLIValue>& valueList = options.getListValues(option);
    values.reserve(valueList.size());
    for (const cli::args::CLIValue& value : valueList) {
        values.push_back(fs::Path(value.getStringValue()));
    }
    return values;
}
std::vector<std::string_view> getStringValues(const cli::args::CLIOptions& options, cli::args::CLIOption option) {
    std::vector<std::string_view> values;
    const std::vector<cli::args::CLIValue>& valueList = options.getListValues(option);
    values.reserve(valueList.size());
    for (const cli::args::CLIValue& value : valueList) {
        values.push_back(value.getStringValue());
    }
    return values;
}

fs::Path getPathValueOr(const cli::args::CLIOptions& options, cli::args::CLIOption option, const fs::Path& defaultValue) {
    const cli::args::CLIValue* value = options.getValue(option);
    if (value != nullptr) {
        return fs::Path(value->getStringValue());
    }
    return defaultValue;
}
std::string_view getStringValueOr(const cli::args::CLIOptions& options, cli::args::CLIOption option, std::string_view defaultValue) {
    const cli::args::CLIValue* value = options.getValue(option);
    if (value != nullptr) {
        return value->getStringValue();
    }
    return defaultValue;
}

}

CompilationConfig CompilationConfig::fromCLIOptions(const cli::args::CLIOptions& options) {
    using CLIOption = cli::args::CLIOption;
    
    CompilationConfig config;

    // TODO: Use actual value defaults

    // Populate config from options
    config.inputFiles = getPathValues(options, CLIOption::InputFile);
    config.outputFile = getPathValueOr(options, CLIOption::OutputFile, fs::Path("out.bin"));

    config.outputMirDirectory = getPathValueOr(options, CLIOption::OutputMir, fs::Path(""));

    return config;
}

} // namespace compilation
VEEC_NAMESPACE_END
