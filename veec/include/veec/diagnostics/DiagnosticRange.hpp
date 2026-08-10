/**
 * @file DiagnosticRange.hpp
 * @brief This file contains the definition of the DiagnosticRange
 * class.
 * 
 * The diagnostic range represents a generic range of text that corresponds to a diagnostic.
 * In most cases, this will be a range of source code, however, it could also be a range of
 * text in a command-line argument, or any other text-based input.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/source/SourceRange.hpp"
#include "veec/diagnostics/DiagnosticSource.hpp"

VEEC_NAMESPACE_BEGIN
namespace diagnostics {

/**
 * @brief Represents a range of text that corresponds to a diagnostic.
 * @note DiagnosticRange enforces that end > begin.
 */
class DiagnosticRange {
public:
    /**
     * @brief Creates a new DiagnosticRange instance with the given source and offsets.
     * @param source The diagnostic source this range belongs to.
     * @param begin The starting offset of the range in the diagnostic source.
     * @param end The ending offset of the range in the diagnostic source.
     */
    DiagnosticRange(const DiagnosticSource* source, u32 begin, u32 end)
        : _source(source), _begin(begin), _end(end) {
        VEE_ASSERT(_source != nullptr, "DiagnosticRange source cannot be null");
        VEE_ASSERT(_end >= _begin, "DiagnosticRange end must be >= to begin");
    }

    /**
     * @brief Creates a DiagnosticRange from a SourceRange.
     * @param range The SourceRange to convert to a DiagnosticRange.
     * @return A DiagnosticRange corresponding to the given SourceRange.
     */
    static DiagnosticRange fromSourceRange(const source::SourceRange& range);

    /**
     * @brief Gets the text of this DiagnosticRange.
     * @return A string_view of the text corresponding to this DiagnosticRange.
     */
    std::string_view getText() const {
        return _source->getDiagnosticRangeText(*this);
    }

    /**
     * @brief Gets the diagnostic source associated with this range.
     * @return The diagnostic source associated with this range.
     */
    const DiagnosticSource* getSource() const { return _source; }
    /**
     * @brief Gets the starting offset of this DiagnosticRange in the diagnostic source.
     * @return The starting offset of this DiagnosticRange.
     */
    u32 getBegin() const { return _begin; }
    /**
     * @brief Gets the ending offset of this DiagnosticRange in the diagnostic source.
     * @return The ending offset of this DiagnosticRange.
     */
    u32 getEnd() const { return _end; }

private:
    const DiagnosticSource* _source;
    u32 _begin;
    u32 _end;
};

} // namespace diagnostics
VEEC_NAMESPACE_END
