/**
 * @file DiagnosticEngine.hpp
 * @brief This file contains the definition of the DiagnosticEngine class.
 * 
 * The DiagnosticEngine is responsible for managing user-facing diagnostics
 * and providing an easy to use interface for reporting them.
 */

#pragma once

#include <vector>
#include <format>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Token.hpp"
#include "veec/source/SourceLocation.hpp"
#include "veec/diagnostics/UserDiagnostic.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"

VEEC_NAMESPACE_BEGIN
namespace diagnostics {

/**
 * @class DiagnosticEngine
 * @brief Manages user facing diagnostics and provides an easy-to-use interface for working
 * with them.
 */
class DiagnosticEngine {
public:
    /// @brief Creates a new DiagnosticEngine instance.
    DiagnosticEngine() = default;
    ~DiagnosticEngine() = default;

    /**
     * @brief Reports a diagnostic to the engine.
     * @param descriptor The descriptor for the diagnostic to report,
     * Get this from the diagnostic catalog.
     * @param range The source range that corresponds to this diagnostic.
     * @param args The arguments to format into the diagnostic message template, if any.
     */
    template<size_t ArgCount, typename... Args>
    inline void report(DiagnosticDescriptor<ArgCount> descriptor, source::SourceRange range, const Args&... args) {
        static_assert(sizeof...(Args) == ArgCount,
            "Argument count does not match descriptor");

        // Format diagnostic and add to vector
        UserDiagnostic d = UserDiagnostic(
            descriptor.kind,
            range,
            descriptor.code,
            std::vformat(descriptor.templateMsg, std::make_format_args(args...))
        );

        _diagnostics.push_back(std::move(d));
    }
    
    /**
     * @brief Sets the error level for this diagnostic engine.
     * For example, if the error level is set to Warning,
     * then both errors and warnings will be considered errors.
     * @param level The error level for this diagnostic engine.
     */
    inline void setErrorLevel(UserDiagnosticKind level) {
        _errorLevel = level;
    }

    /**
     * @brief Gets the list of diagnostics this diagnostic engine contains.
     * @return The list of diagnostics this diagnostic engine contains (read-only).
     */
    inline const std::vector<UserDiagnostic>& getDiagnostics() const {
        return _diagnostics;
    }

    /**
     * @brief Checks if this diagnostic engine contains any reported errors.
     * Errors are determined by the current error level of the diagnostic engine.
     * @return True if this diagnostic engine contains any reported errors, false otherwise.
     */
    inline bool hasErrors() const {
        for (const auto& d : _diagnostics) {
            if (d.getKind() <= _errorLevel) {
                return true;
            }
        }
        return false;
    }

private:
    std::vector<UserDiagnostic> _diagnostics;
    UserDiagnosticKind _errorLevel = UserDiagnosticKind::Error;
};

} // namespace diagnostics
VEEC_NAMESPACE_END
