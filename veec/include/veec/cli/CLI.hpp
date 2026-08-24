/**
 * @file CLI.hpp
 * @brief This file contains the definition of the root CLI descriptor, related types and
 * utilities.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/CLIContext.hpp"
#include "veec/cli/CLIInvocation.hpp"
#include "veec/cli/CLICommandOptions.hpp"
#include "veec/cli/descriptor/CLIDescriptorFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

/**
 * @class CLI
 * @brief The CLI class is the primary interface for interacting with the application command-line pipeline.
 * It handles parsing arguments, dispatching commands, and managing the overall CLI flow.
 */
class CLI {
public:
    CLI() = default;
    ~CLI() = default;

    /**
     * @brief Parses the raw command-line arguments into a CLIInvocation object.
     * @return A CLIInvocation object representing the parsed command and its associated options.
     * Returns an invalid CLIInvocation if parsing fails or if the command is unknown.
     */
    CLIInvocation parse(int argc, const char** argv);

	/**
	 * @brief Retrieves the diagnostics generated from parsing.
	 * @return A list of diagnostics.
	 */
	const std::vector<diagnostics::UserDiagnostic>& getDiagnostics() const {
		return _ctx.diagnostics.getDiagnostics();
	}

    /**
     * @brief Dispatches the given CLIInvocation to the appropriate command handler.
     * @param invocation The CLIInvocation to dispatch. MUST be valid.
     */
    int dispatch(const CLIInvocation& invocation);

private:
    CLIContext _ctx;
};

} // namespace cli
VEEC_NAMESPACE_END
