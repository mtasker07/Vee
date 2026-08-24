#include "veec/compilation/CompilationConfig.hpp"

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/CLIOption.hpp"
#include "veec/cli/CLICommandOptions.hpp"

VEEC_NAMESPACE_BEGIN
namespace compilation {

namespace {

std::vector<fs::Path> getPathValues(const cli::CLICommandOptions& options, cli::CLIOption option) {
    std::vector<fs::Path> values;
    const std::vector<cli::CLIValue>& valueList = options.getListValues(option);
    values.reserve(valueList.size());
    for (const cli::CLIValue& value : valueList) {
        values.push_back(fs::Path(value.getStringValue()));
    }
    return values;
}
std::vector<std::string_view> getStringValues(const cli::CLICommandOptions& options, cli::CLIOption option) {
    std::vector<std::string_view> values;
    const std::vector<cli::CLIValue>& valueList = options.getListValues(option);
    values.reserve(valueList.size());
    for (const cli::CLIValue& value : valueList) {
        values.push_back(value.getStringValue());
    }
    return values;
}

fs::Path getPathValueOr(const cli::CLICommandOptions& options, cli::CLIOption option, const fs::Path& defaultValue) {
    const cli::CLIValue* value = options.getValue(option);
    if (value != nullptr) {
        return fs::Path(value->getStringValue());
    }
    return defaultValue;
}
std::string_view getStringValueOr(const cli::CLICommandOptions& options, cli::CLIOption option, std::string_view defaultValue) {
    const cli::CLIValue* value = options.getValue(option);
    if (value != nullptr) {
        return value->getStringValue();
    }
    return defaultValue;
}

}

CompilationConfig CompilationConfig::fromCLIOptions(const cli::CLICommandOptions& options) {
    using CLIOption = cli::CLIOption;
    
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
