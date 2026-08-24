/**
 * @file CLIRawArgs.hpp
 * @brief This file contains the definition of the CLIRawArgs class.
 * 
 * The CLIRawArgs class is a wrapper around raw command-line arguments (argc, argv) and provides
 * helpful utilities for using them.
 */

#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/CLICommandOptions.hpp"
#include "veec/diagnostics/DiagnosticSource.hpp"
#include "veec/diagnostics/DiagnosticRange.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

/**
 * @class CLIRawArgs
 * @brief A wrapper around raw command-line arguments (argc, argv).
 */
class CLIRawArgs : public diagnostics::DiagnosticSource {
public:
	/**
	 * @brief Creates a new empty CLIRawArgs instance with no arguments.
	 */
    CLIRawArgs() = default;
    /**
     * @brief Creates a new CLIRawArgs instance from raw command-line arguments.
     */
    CLIRawArgs(int argc, const char** argv) {
        parse(argc, argv);
    }

    ~CLIRawArgs() = default;

    /**
     * @brief Parses the raw command-line arguments (argc, argv) and stores them in the CLIRawArgs instance.
     * @param argc The number of command-line arguments.
     * @param argv The array of command-line argument strings.
     */
    void parse(int argc, const char** argv);

    /**
     * @brief Gets the number of arguments stored in this CLIRawArgs instance.
     * @return The number of arguments.
     */
    inline size_t getArgCount() const {
        return _args.size();
    }
    /**
     * @brief Gets the list of argument values stored in this CLIRawArgs instance.
     * @return A vector of string_views representing the argument values.
     */
    inline const std::vector<std::string_view>& getArgValues() const {
        return _args;
    }
    /**
     * @brief Gets the argument at the specified index.
     * @param index The index of the argument to retrieve.
     * @return A string_view of the argument at the specified index.
     */
    inline std::string_view getArgValue(size_t index) const {
        VEE_ASSERT(index < _args.size(), "Argument index out of bounds");
        return _args[index];
    }

    /**
     * @brief Gets the diagnostic range for a given argument index.
     * For diagnostics, arguments are formatted together, so use this function to grab
     * the accurate range of an argument before reporting. 
     * @param argIndex The index of the argument to get the diagnostic range for.
     * @return A DiagnosticRange corresponding to the argument at the given index.
     */
    diagnostics::DiagnosticRange getArgRange(size_t argIndex) const;
    /**
     * @brief Gets the diagnostic range for global diagnostids. In other words,
     * the range for diagnostics that are not tied to a specific argument.
     * @return A DiagnosticRange that corresponds to the generic arguments list.
     */
    diagnostics::DiagnosticRange getGlobalRange() const;

    //
    // DiagnosticSource
    //

    inline diagnostics::DiagnosticSourceKind getDiagnosticSourceKind() const override {
        return diagnostics::DiagnosticSourceKind::GenericText;
    }
    inline std::string_view getDiagnosticRangeText(diagnostics::DiagnosticRange range) const override {
        size_t begin = range.getBegin();
        size_t end = range.getEnd();

        VEE_ASSERT(begin <= end, "Range out of bounds: begin must be less than or equal to end");
        VEE_ASSERT(end <= _formattedArgs.size(), "Range out of bounds: must be less than the size of the string");

        return std::string_view(&_formattedArgs[begin], end - begin);
    }

private:
    std::vector<std::string_view> _args;
    // ^^ Does not include program path
    // vv Includes program path
    std::string _formattedArgs;
    std::vector<std::pair<u32, u32>> _formattedArgOffsets;

    void makeFormattedArgs(int argc, const char** argv);
    std::string formatArg(std::string_view arg) const;
};

} // namespace cli
VEEC_NAMESPACE_END
