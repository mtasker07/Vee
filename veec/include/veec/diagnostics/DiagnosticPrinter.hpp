/**
 * @file DiagnosticPrinter.hpp
 * @brief This file contains the definition of the DiagnosticPrinter class.
 * The DiagnosticPrinter is a helper class for rendering diagnostics to the terminal or other output streams.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/io/StreamWriter.hpp"
#include "veec/diagnostics/UserDiagnostic.hpp"
#include "veec/diagnostics/rendering/TerminalDiagnosticRenderer.hpp"

VEEC_NAMESPACE_BEGIN
namespace diagnostics {

/**
 * @class DiagnosticPrinter
 * @brief Helper class for printing diagnostics to the terminal or other output streams.
 * @tparam Renderer The renderer to use for rendering diagnostics. Defaults to TerminalDiagnosticRenderer.
 */
template<typename Renderer = TerminalDiagnosticRenderer>
class DiagnosticPrinter {
public:
    DiagnosticPrinter() = default;
    ~DiagnosticPrinter() = default;

    /**
     * @brief Prints a list of diagnostics to the given output stream.
     * @param diagnostics The list of diagnostics to print.
     * @param writer The output stream to print the diagnostics to.
     * @param spaceBetween Whether to add a space between diagnostics. Defaults to true.
     */
    inline void printDiagnostics(const std::vector<UserDiagnostic>& diagnostics, io::StreamWriter& writer, bool spaceBetween = true) const {
        for (const auto& diagnostic : diagnostics) {
            printDiagnostic(diagnostic, writer);
            if (spaceBetween) writer.write("\n");
        }
    }
    /**
     * @brief Prints a given diagnostic to the given output stream.
     * @param diagnostic The diagnostic to print.
     * @param writer The output stream to print the diagnostic to.
     */
    inline void printDiagnostic(const UserDiagnostic& diagnostic, io::StreamWriter& writer) const {
        _renderer.render(diagnostic, writer);
    }

private:
    Renderer _renderer;
};

} // namespace diagnostics
VEEC_NAMESPACE_END
