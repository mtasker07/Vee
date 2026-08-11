#include "veec/cli/args/CLIArgs.hpp"

#include <string>
#include <string_view>
#include <vector>
#include <format>
#include <filesystem>
#include <algorithm>
#include <utility>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/cli/args/CLIOptions.hpp"
#include "veec/cli/args/CLIOptionsGenerator.hpp"
#include "veec/diagnostics/DiagnosticRange.hpp"
#include "veec/diagnostics/DiagnosticSource.hpp"
#include "veec/util/CharUtils.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace args {

void CLIArgs::parse(int argc, const char** argv) {
	makeFormattedArgs(argc, argv);

	_args.clear();

	if (argc <= 0 || argv == nullptr) {
		return;
	}

	// Skip argv[0] (program path) for _args
	if (argc <= 1) {
		return;
	}

	_args.reserve(static_cast<size_t>(argc - 1));
	for (int i = 1; i < argc; ++i) {
		std::string_view arg = argv[i] != nullptr
			? std::string_view(argv[i])
			: std::string_view();

		_args.push_back(arg);
	}
}

CLIOptions CLIArgs::generateOptions(compilation::CompilationContext& ctx) const {
	CLIOptionsGenerator generator(ctx);
	return generator.generateFromArgs(*this);
}

diagnostics::DiagnosticRange CLIArgs::getArgRange(size_t argIndex) const {
    VEE_ASSERT(argIndex < _formattedArgOffsets.size(), "Argument index out of bounds");

	u32 begin = _formattedArgOffsets[argIndex].first;
	u32 end = _formattedArgOffsets[argIndex].second;

	return diagnostics::DiagnosticRange(this, begin, end);
}
diagnostics::DiagnosticRange CLIArgs::getGlobalRange() const {
	size_t begin = 0;
	size_t end = _formattedArgs.size();

	return diagnostics::DiagnosticRange(this, static_cast<u32>(begin), static_cast<u32>(end));
}

void CLIArgs::makeFormattedArgs(int argc, const char** argv) {
	_formattedArgs.clear();
    _formattedArgOffsets.clear();

	// No args (technically impossible?)
	if (argc <= 0 || argv == nullptr) {
		return;
	}

	_formattedArgOffsets.reserve(static_cast<size_t>(argc - 1));

	// Program path is formatted differently since its usually the full
	// path which can look ugly in diagnostics
	std::filesystem::path programPath = argv[0];
	if (programPath.has_filename()) {
		// Try and print filename only
		_formattedArgs += formatArg(programPath.filename().string());
	}
	else {
		// Fallback full path
		_formattedArgs += formatArg(programPath.string());
	}
	// Program path offset
	_formattedArgOffsets.push_back({ 0, static_cast<u32>(_formattedArgs.size()) });

	// For the rest of the args, format them and join with spaces
	for (int i = 1; i < argc; ++i) {
		std::string_view arg = argv[i] != nullptr
			? std::string_view(argv[i])
			: std::string_view();

		_formattedArgs += " ";
		u32 begin = static_cast<u32>(_formattedArgs.size());
		_formattedArgs += formatArg(arg);
		u32 end = static_cast<u32>(_formattedArgs.size());
		_formattedArgOffsets.push_back({ begin, end });
	}
}
std::string CLIArgs::formatArg(std::string_view arg) const {
	if (arg.empty()) {
		return "\"\"";
	}

	// Quote if whitespace is present
	if (std::any_of(arg.begin(), arg.end(), [](char c) { return util::CharUtils::isWhitespace(c); })) {
		return std::format("\"{}\"", arg);
	}

	return std::string(arg);
}

} // namespace args
} // namespace cli
VEEC_NAMESPACE_END
