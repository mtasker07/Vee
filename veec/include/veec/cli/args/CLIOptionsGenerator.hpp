/**
 * @file CLIOptionsGenerator.hpp
 * @brief This file contains the definition of the CLIOptionsGenerator class.
 * 
 * The CLIOptionsGenerator class is responsible for generating CLIOptions instances.
 */

#pragma once

#include <string_view>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/args/CLIOptions.hpp"
#include "veec/cli/args/CLIArgs.hpp"

VEEC_NAMESPACE_BEGIN

namespace compilation {
    class CompilationContext;
}

namespace cli {
namespace args {

class CLIOptions;

class CLIOptionsGenerator {
public:
    CLIOptionsGenerator(compilation::CompilationContext& ctx)
        : _ctx(ctx) {}

    CLIOptions generateFromArgs(const CLIArgs& args);

private:
    compilation::CompilationContext& _ctx;
};

} // namespace args
} // namespace cli
VEEC_NAMESPACE_END
