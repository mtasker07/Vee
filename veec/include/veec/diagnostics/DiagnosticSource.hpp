/**
 * @file DiagnosticSource.hpp
 * @brief This file contains the definition of the DiagnosticSource
 * class.
 * 
 * The diagnostic source represents the origin of a diagnostic message,
 * such as a source file or text snippet.
 *
 */

#pragma once

#include <string>
#include <format>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/source/SourceRange.hpp"

VEEC_NAMESPACE_BEGIN
namespace diagnostics {

class DiagnosticRange;

/**
 * @brief Represents the kind of a diagnostic source, which changes the way it gets formatted
 * and displayed to the user.
 */
enum class DiagnosticSourceKind {
    /**
     * @brief This diagnostic source is a snippet of source code.
     */
    SourceCode,
    /**
     * @brief This diagnostic source is a generic piece of text.
     */
    GenericText
};

/**
 * @brief Base class for all diagnostic sources. A diagnostic source is any text representation
 * that can be used to provide context for a diagnostic message. Most commonly this will be a
 * source file.
 */
class DiagnosticSource {
public:
    virtual ~DiagnosticSource() = default;

    /**
     * @brief Gets the kind of this diagnostic source.
     * @return The kind of this diagnostic source.
     */
    virtual DiagnosticSourceKind getDiagnosticSourceKind() const = 0;
    /**
     * @brief Gets the text that corresponds to a given diagnostic range.
     * @param range The diagnostic range to get the text of.
     * @return A string_view of the text corresponding to the given diagnostic range.
     */
    virtual std::string_view getDiagnosticRangeText(DiagnosticRange range) const = 0;
};

} // namespace diagnostics
VEEC_NAMESPACE_END
