/**
 * @file CLIContext.hpp
 * @brief This file contains the definition of the CLIContext class,
 * which is used to hold context for the CLI.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/CLIRawArgs.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

/**
 * @class CLIContext
 * @brief Used to hold context for the CLI.
 */
class CLIContext {
public:
    CLIRawArgs args;
    diagnostics::DiagnosticEngine diagnostics;

    CLIContext() = default;
    ~CLIContext() = default;
};

} // namespace cli
VEEC_NAMESPACE_END
