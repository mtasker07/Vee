/**
 * @file TerminalDiagnosticRenderer.hpp
 * @brief This file contains the definition of the TerminalDiagnosticRenderer class.
 * 
 * The TerminalDiagnosticRenderer is responsible for converting diagnostics into an informative and user friendly text representation that
 * can be displayed to the user via the terminal.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/io/StreamWriter.hpp"
#include "veec/diagnostics/DiagnosticSource.hpp"
#include "veec/diagnostics/DiagnosticRange.hpp"
#include "veec/diagnostics/UserDiagnostic.hpp"
#include "veec/diagnostics/UserDiagnosticNote.hpp"

VEEC_NAMESPACE_BEGIN
namespace diagnostics {

/**
 * @class TerminalDiagnosticRenderer
 * @brief Responsible for converting diagnostics into an informative and user friendly text representation that can be
 * displayed to the user via the terminal.
 */
class TerminalDiagnosticRenderer {
public:
    TerminalDiagnosticRenderer() = default;
    ~TerminalDiagnosticRenderer() = default;

    /**
     * @brief Renders a diagnostic to the terminal.
     * @param diagnostic The diagnostic to render.
     * @param terminalWriter The terminal writer to use for output.
     * @note If you want to render to a generic output stream, you can pass any StreamWriter as the terminalWriter, however, make sure
     * you disable all the styles as they will most likely not be supported by generic output streams.
     */
    void render(const UserDiagnostic& diagnostic, io::StreamWriter& terminalWriter) const;

private:
    void renderSourceCode(const UserDiagnostic& diagnostic, io::StreamWriter& terminalWriter) const;
    void renderGenericText(const UserDiagnostic& diagnostic, io::StreamWriter& terminalWriter) const;

    void renderNoteSourceCode(const UserDiagnosticNote& note, io::StreamWriter& terminalWriter) const;
    void renderNoteGenericText(const UserDiagnosticNote& note, io::StreamWriter& terminalWriter) const;

    // Helpers
    void writeLine(std::string_view line, io::StreamWriter& terminalWriter) const;
};

} // namespace diagnostics
VEEC_NAMESPACE_END
