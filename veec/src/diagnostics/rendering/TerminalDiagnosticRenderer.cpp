#include "veec/diagnostics/rendering/TerminalDiagnosticRenderer.hpp"

#include <string>
#include <string_view>
#include <format>
#include <algorithm>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/io/StreamWriter.hpp"
#include "veec/source/SourceFile.hpp"
#include "veec/diagnostics/UserDiagnostic.hpp"

VEEC_NAMESPACE_BEGIN
namespace diagnostics {

void TerminalDiagnosticRenderer::render(const UserDiagnostic& diagnostic, io::StreamWriter& terminalWriter) const {
    switch (diagnostic.getRange().getSource()->getDiagnosticSourceKind()) {
        case DiagnosticSourceKind::SourceCode:
            renderSourceCode(diagnostic, terminalWriter);
            break;

        case DiagnosticSourceKind::GenericText:
            renderGenericText(diagnostic, terminalWriter);
            break;

        default:
            VEE_UNREACHABLE("Invalid DiagnosticSourceKind");
    }
}

void TerminalDiagnosticRenderer::renderSourceCode(const UserDiagnostic& diagnostic, io::StreamWriter& terminalWriter) const {
    // Format:
    //
    // {filepath}:{line}:{column} - {KIND} {code}: {message}
    //
    // {context}
    //    ^~~~~~ HERE
    //
    // {notes}

    std::string_view kindStr = toString(diagnostic.getKind());
    std::string_view filePath = "<unknown>";
    u32 line = 0;
    u32 column = 0;
    std::vector<std::string_view> contextLines;

    u32 contextStartLine = 0;
    u32 contextEndLine = 0;

    const source::SourceFile* sourceFile = dynamic_cast<const source::SourceFile*>(diagnostic.getRange().getSource());
    if (sourceFile) {
        filePath = sourceFile->getPath().str();

        source::LineColumn lineCol = sourceFile->getLineColumn(diagnostic.getRange().getBegin());
        line = lineCol.line;
        column = lineCol.column;

        u32 totalLines = static_cast<u32>(sourceFile->getLineCount());

        contextStartLine = std::max(1u, static_cast<u32>(line) - 2);
        contextEndLine = std::min(totalLines, static_cast<u32>(line) + 1);
    }

    std::string lineStr = line == 0 ? "?" : std::to_string(line);
    std::string columnStr = column == 0 ? "?" : std::to_string(column);

    // Header
    writeLine(std::format(
        "{}:{}:{} - {} {}: {}",
        filePath,
        lineStr,
        columnStr,
        kindStr,
        diagnostic.getCode(),
        diagnostic.getMessage()
    ), terminalWriter);

    // Context
    size_t lnoWidth = std::to_string(contextEndLine).size();
    for (u32 i = contextStartLine; i <= contextEndLine; ++i) {
        std::string_view lineText = sourceFile->getLineText(i, false);
        writeLine(std::format("{:>{}} | {}", i, lnoWidth, lineText), terminalWriter);

        if (i == line) {
            const diagnostics::DiagnosticRange range = diagnostic.getRange();
            const size_t rangeLen = std::max<size_t>(1, static_cast<size_t>(range.getEnd() - range.getBegin()));
            const size_t caretOffset = static_cast<size_t>(column - 1);
            const size_t lineLen = lineText.size();

            size_t maxHighlightLen = 1;
            if (caretOffset < lineLen) {
                maxHighlightLen = lineLen - caretOffset;
            }

            const size_t highlightLen = std::min(rangeLen, maxHighlightLen);
            std::string indicator(column - 1, ' ');
            indicator += "^";
            if (highlightLen > 1) {
                indicator += std::string(highlightLen - 1, '~');
            }
            indicator += " HERE";
            writeLine(std::format("{:>{}} | {}", "", lnoWidth, indicator), terminalWriter);
        }
    }
}
void TerminalDiagnosticRenderer::renderGenericText(const UserDiagnostic& diagnostic, io::StreamWriter& terminalWriter) const {
    // Format:
    //
    // {KIND} {code}: {message}
    // <context>
    // ^~~~~~ HERE

    std::string_view kindStr = toString(diagnostic.getKind());
    diagnostics::DiagnosticRange range = diagnostic.getRange();
    
    const u32 begin = range.getBegin();
    const u32 end = range.getEnd();
    
    writeLine(std::format(
        "{} {}: {}",
        kindStr,
        diagnostic.getCode(),
        diagnostic.getMessage()
    ), terminalWriter);
    
    // Dont show context for empty ranges
    if (begin == end) {
        return;
    }

    std::string_view context = range.getText();
    writeLine(context, terminalWriter);

    const size_t highlightLen = std::max<size_t>(1, static_cast<size_t>(end - begin));
    std::string indicator = "^";
    if (highlightLen > 1) {
        indicator += std::string(highlightLen - 1, '~');
    }
    indicator += " HERE";
    writeLine(indicator, terminalWriter);
}

void TerminalDiagnosticRenderer::writeLine(std::string_view line, io::StreamWriter& terminalWriter) const {
    terminalWriter.write(line);
    terminalWriter.write("\n");
}

} // namespace diagnostics
VEEC_NAMESPACE_END
