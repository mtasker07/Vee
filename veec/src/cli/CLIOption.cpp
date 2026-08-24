#include "veec/cli/CLIOption.hpp"

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/fs/Path.hpp"
#include "veec/cli/CLIOption.hpp"
#include "veec/cli/descriptor/CLIOptionDescriptor.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

bool isFlagOption(const CLIOption option) {
	return descriptor::isFlagOption(*descriptor::getOptionDescriptor(option));
}
bool isCounterOption(const CLIOption option) {
	return descriptor::isCounterOption(*descriptor::getOptionDescriptor(option));
}
bool isValueOption(const CLIOption option) {
	return descriptor::isValueOption(*descriptor::getOptionDescriptor(option));
}
bool isListOption(const CLIOption option) {
	return descriptor::isListOption(*descriptor::getOptionDescriptor(option));
}

} // namespace cli
VEEC_NAMESPACE_END
