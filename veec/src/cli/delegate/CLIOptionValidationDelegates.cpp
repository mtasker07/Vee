#include "veec/cli/delegate/CLIOptionValidationDelegates.hpp"

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/fs/Path.hpp"
#include "veec/cli/CLIValue.hpp"
#include "veec/cli/descriptor/CLIOptionDescriptor.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace delegate {

namespace {

bool validateValidFilePath(const CLIValue& value, bool ensureExists) {
    std::string_view pathStr = value.getStringValue();
    fs::Path path = fs::Path(pathStr);

    if (path.isEmpty()) {
        return false;
    }

    if (ensureExists && (!path.exists() || !path.isFile())) {
        return false;
    }

    return true;
}
bool validateValidDirectoryPath(const CLIValue& value, bool ensureExists) {
	std::string_view pathStr = value.getStringValue();
	fs::Path path = fs::Path(pathStr);
	if (path.isEmpty()) {
		return false;
	}

	if (ensureExists && (!path.exists() || !path.isDirectory())) {
		return false;
	}

	return true;
}

} // namespace

//
// Option Validators
//

// Input file
bool inputFileValidationDelegate(descriptor::CLIOptionValidationContext& ctx, const CLIValue& value) {
	if (!validateValidFilePath(value, true)) {
		// Check again passing false to identify the specific issue (bad path vs doesnt exist)
		diagnostics::DiagnosticDescriptor<1> descriptor = validateValidFilePath(value, false) ?
			diagnostics::ERROR_CLI_FILE_DOESNT_EXIST :
			diagnostics::ERROR_CLI_NOT_A_FILE;

		ctx.diagnostics.report(
			descriptor,
			ctx.valueRange,
			value.getStringValue()
		);
		return false;
	}
	
	return true;
}

// Output file
bool outputFileValidationDelegate(descriptor::CLIOptionValidationContext& ctx, const CLIValue& value) {
	if (!validateValidFilePath(value, false)) {
		ctx.diagnostics.report(
			diagnostics::ERROR_CLI_NOT_A_FILE,
			ctx.valueRange,
			value.getStringValue()
		);
		return false;
	}

	return true;
}

// Optimization level
bool optimizationLevelValidationDelegate(descriptor::CLIOptionValidationContext& ctx, const CLIValue& value) {
	auto reportValueOutOfRange = [&]() {
		ctx.diagnostics.report(
			diagnostics::ERROR_CLI_INVALID_VALUE_WITH_MSG,
			ctx.valueRange,
			value.getStringValue(),
			descriptor::getOptionFullName(ctx.descriptor),
			"optimization level must be an integer between 0 and 3"
		);
	};

	basic::BigInt bigIntValue = value.getIntegerValue();
	if (!bigIntValue.fitsInUnsigned(32)) {
		reportValueOutOfRange();
		return false;
	}

	u32 intValue = bigIntValue.toU32();
	if (intValue < 0 || intValue > 3) {
		reportValueOutOfRange();
		return false;
	}

	return true;
}

// Output MIR
bool mirOutputDirectoryValidationDelegate(descriptor::CLIOptionValidationContext& ctx, const CLIValue& value) {
	if (!validateValidDirectoryPath(value, false)) {
		// Check again passing false to identify the specific issue (bad path vs doesnt exist)
		diagnostics::DiagnosticDescriptor<1> descriptor = validateValidDirectoryPath(value, false) ?
			diagnostics::ERROR_CLI_DIRECTORY_DOESNT_EXIST :
			diagnostics::ERROR_CLI_NOT_A_DIRECTORY;
		ctx.diagnostics.report(
			descriptor,
			ctx.valueRange,
			value.getStringValue()
		);
		return false;
	}
	return true;
}

} // namespace delegate
} // namespace cli
VEEC_NAMESPACE_END
