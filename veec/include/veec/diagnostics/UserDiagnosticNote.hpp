/**
 * @file UserDiagnosticNote.hpp
 * @brief This file contains the definition of the UserDiagnosticNote
 * class.
 *
 * The user diagnostic note represents a note attached to a user diagnostic,
 * such as additional context or information.
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
 * @class UserDiagnosticNote
 * @brief Represents a note attached to a user diagnostic.
 */
class UserDiagnosticNote {
public:
    /**
     * @brief Constructs a new UserDiagnosticNote instance with the given range, code, and message.
     * @param range The range of text the note corresponds to.
     * @param code The note code (optional).
     * @param message The note message.
     */
    UserDiagnosticNote(
        DiagnosticRange range,
        u16 code,
        std::string message
    )
        : _range(range),
        _code(code),
        _message(std::move(message)) {}

    /**
     * @brief Gets the range of this note.
     * @return The range of this note.
     */
    DiagnosticRange getRange() const { return _range; }
    /**
     * @brief Gets the code of this note.
     * @return The code of this note.
     */
    u16 getCode() const { return _code; }
    /**
     * @brief Gets the message of this note.
     * @return The message of this note.
     */
    std::string_view getMessage() const { return _message; }

private:
    DiagnosticRange _range;
    u16 _code;
    std::string _message;
};

} // namespace diagnostics
VEEC_NAMESPACE_END
