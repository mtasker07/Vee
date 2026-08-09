/**
 * @file UserDiagnostic.hpp
 * @brief This file contains the definition of the UserDiagnostic
 * struct.
 * 
 * The user diagnostic represents a diagnostic message that should be displayed to the user,
 * such as an error or warning.
 * 
 * @note It's important that the user diagnostic is not used for internal diagnostics.
 * Instead use the internal error handling system in `vee/core/InternalErrorHandling.hpp`.
 * Specifically: VEE_FATAL(), VEE_ASSERT(), VEE_UNREACHABLE()...
 */

#pragma once

#include <string>
#include <string_view>
#include <format>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/diagnostics/DiagnosticRange.hpp"

VEEC_NAMESPACE_BEGIN
namespace diagnostics {

/**
 * @enum UserDiagnosticKind
 * @brief Represents the kind of a user diagnostic.
 */
enum class UserDiagnosticKind {
    Error,
    Warning,
    Info,
};

inline std::string_view toString(UserDiagnosticKind kind) {
    switch (kind) {
        case UserDiagnosticKind::Error: return "error";
        case UserDiagnosticKind::Warning: return "warning";
        case UserDiagnosticKind::Info: return "info";
        default:
            VEE_UNREACHABLE("Invalid UserDiagnosticKind");
    }
}

/**
 * @struct UserDiagnostic
 * @brief Represents a diagnostic message that should be displayed to the user.
 */
class UserDiagnostic {
public:
    /**
     * @brief Constructs a new UserDiagnostic instance with the given kind, range, code, and message.
     * @param kind The kind of the diagnostic (error, warning, info).
     * @param range The range of text the diagnostic corresponds to.
     * @param code The diagnostic code (optional).
     * @param message The diagnostic message.
     */
    UserDiagnostic(
        UserDiagnosticKind kind,
        DiagnosticRange range,
        u16 code,
        std::string message
    )
        : _kind(kind),
        _range(range),
        _code(code),
        _message(std::move(message)) {}

    /**
     * @brief Gets the kind of this diagnostic.
     * @return The kind of this diagnostic.
     */
    UserDiagnosticKind getKind() const { return _kind; }
    /**
     * @brief Gets the range of this diagnostic.
     * @return The range of this diagnostic.
     */
    DiagnosticRange getRange() const { return _range; }
    /**
     * @brief Gets the diagnostic code of this diagnostic.
     * @return The diagnostic code of this diagnostic.
     */
    u16 getCode() const { return _code; }
    /**
     * @brief Gets the diagnostic message of this diagnostic.
     * @return The diagnostic message of this diagnostic.
     */
    std::string_view getMessage() const { return _message; }

    /**
     * @brief Converts this diagnostic to a string for display to the user.
     * @return A string representation of this diagnostic.
     */
    std::string toString() const {
        std::string_view context = _range.getText(); // (TEMPORARY)
        return std::format("[{}:{}] {}: {}\nContext:\n{}", _range.getBegin(), _range.getEnd(), diagnostics::toString(_kind), _message, context);
    }

private:
    UserDiagnosticKind _kind;
    DiagnosticRange _range;
    u16 _code;
    std::string _message;
};

} // namespace diagnostics
VEEC_NAMESPACE_END
